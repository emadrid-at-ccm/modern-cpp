# How to handle errors

The subject of errors is of capital importance, in my experience not doing this right is what leads to the vast majority of bugs and wasted programmer efforts.

C++ is particularly good at handling errors.  It gives three very reasonable options, and it also supports error codes.

I had said there were three ways to handle errors, it turns out there are four, I was not fully aware of the monadic way.

Let us cover the four ways:

# Error codes

This is the worst of all possible options, however, it is the most prevalent :disappointed: perhaps because a very poor language like C can only support this way.

Error codes don't really have advantages, but it is important to understand the deficiencies that programmers ignore to their detriment.  Quoting [one of my lecture notes](https://github.com/emadrid-at-ccm/modern-cpp/blob/48c012556d53be7e2770c1e9996586b01ebea786/3.5/Class5-post.md#error-codes), and improving the explanations *in italics*:

0. They require the programmer to consistently check for each function that may fail:
    1. Never done consistently in practice
    2. Introduces a performance penalty, the check itself
    3. Uglyfies the code
1. The nature of errors is unbounded and unknowable at programming time, thus **it is unrealistic that programmers are going to be able to cover for any error that can happen in practice**
    4. Impossible to audit in practice *because error codes are arbitrary codes, in general, the compiler does not know which are the codes a function can return*
    2. It is impossible in practice to test for all possible errors
1. Error codes are just a code or a label for the problem that happened.
    1. The information that describes the error belongs to the execution context *the typical example is `errno`*
        1. Presents complications to make the information related to the error thread safe
        2. It is global to the process, ephemeral: The error information must be checked immediately before calling anything that could overwrite it if there are further errors.  *This forces the program to tend to errors immediately, even if they are not interested in solving them*
2. It is a brittle mechanism: If some function changes and may have a new error code, what is the guarantee all the callers, and the callers of the callers that do not "swallow" the error will correctly deal with the new code?
3. Error codes are notoriously hard to propagate:
    1. The error codes for a funtion becomes the union of the error codes of all of the things that can fail within them
    2. Frequently, two different functions use the same code for different errors, so, error codes need conversion functions
    3. Because the codes themselves are part of the user interface, implementation details change the public interface, exposes user code to the implementation details
    4. For example, in Maystreet's Bellport the error codes became a kitchen sink of everything that could fail in the API.
4. Digression: Most languages implicity define optional semantics using the equivalent of an error code:  Return a `null` reference, `nil`, `none`, or a null pointer.  Don't do this in C++ unless it is really useful, and for that better use [`boost::optional`](http://www.boost.org/doc/libs/1_63_0/libs/optional/doc/html/index.html) or the upcoming [`std::optional`](http://en.cppreference.com/w/cpp/utility/optional)

# Exceptions

Support for exceptions in C++ is wonderful.  I assume the reader knows what exceptions are.

## Optimistic programming

Exceptions allow what I have referred to as "optimistic programming" in other documents.

The user does not have to care about the errors that may occur as long as they follow good practices, in particular:

1. Cleaning up in the destructors and
2. Committing execution environment changes only when the transactions have completed

The exceptions support allow:

1. Transactional semantics to be achievable, or that the programmer can be oblivious (optimistic) with respect to errors, the errors propagate, allowing the code to clean up after itself in a reliable way, up to the point the problem can be resolved.
2. Don't force code that knows little about an error to have to deal with it.
3. It only requires testing for whether the transactional semantics is achieved, which should not be dependent on the nature of the errors that may happen.

With these two things, code that fails due to an exception being raised, won't damage the execution environment, and should be retriable, for no extra cost whatsoever to the programmer.

## Words of practical advice:

* If your code wants to write a record to a file, please make sure it only writes complete records.  Keep a structure with the record partially formed as opposed to write the record partially to file; that way you benefit from the *transactional semantics* allowed by the excellent support for exceptions in C++: If something fails, your file does not get corrupted, you may try writing the record at a later time.
* If you are writing `catch` to cleanup after yourself as opposed to being interested in *solving* the problem, you are doing it wrong.  Put the `catch` code in a destructor.  You will not have to remember to write the cleanup code, the compiler won't forget to put it.  Plus you introduce noise.  `catch` only when you want to try to solve the problem.

This is an interesting topic, I invite the reader to consider these things:

1. C++ does not have a `finally` clause because it does not need it.  `finally` is useful in other languages like Java for cleaning up resources if exceptions happen, the code that gets executed after the `try` regardless.  Proper C++ cleans after itself in the destructors, the destructors are called even when unwinding the stack to propagate exceptions.
2. Currently C++ has what is called "zero cost" support for exceptions, which means that at runtime there is no thunking or framing of the stack to support exceptions, the frames are specified at compilation time, the exception mechanism searches based on the "Program Counter" or "Instruction Pointer" the frame where it is, that frame contains program code to deal with the exception.  This does not mean that exceptions, when they happen are "free of cost", just that *supporting* exceptions adds only the exception frame data to the object code, without any further runtime cost.  This is explained in [The true cost of zero cost exceptions](https://mortoray.com/2013/09/12/the-true-cost-of-zero-cost-exceptions/).
3. There are complications, such as that destructors should not raise exceptions, since destructors are invoked from within exception propagations, supporting destructors that my throw may require the support of the propagation of more than one exception.  In C++ **raising an exception from within the context of an exception being thrown is program termination**.  This is the second of the reasons why destructors are the most important pieces of code (the other being that since they are always called, getting them right is essential for performance reasons).
4. There is a problem with exceptions in general, which is that they are implementation dependent and not related to the intention of the programmer.  This is what motivates even more sophisticated ways to deal with errors.

## Bad designs that abuse exceptions

C++ is not good at returning more than one value from functions.  The situation has gotten better with `std::tie` and `std::tuple`, but it continues to be far from ideal.  Consequently, when a good API should return more than one value, designers may make mistakes.  I think the family of functions [`std::stoi, stol, stof, stod`, etc](http://en.cppreference.com/w/cpp/string/basic_string/stof) are deeply flawed: for the common case where the supplied string is not a number, they raise an exception.  This is eggregious because in any way you would determine if a string is a valid number you may as well convert it to the number; so, it is not possible to prevent the case of frequent exceptions being thrown.  What these functions should do is to return two things: the number and whether they succeeded.  I do this in my scheme for visitations of `any` (more on this below).

Do not make this mistake.  The support for exceptions assumes exceptions are exceptional, as a programming technique they are:

1. Expensive, disruptive and hard to reason about.
2. May transfer control far from when they happen, potentially to program termination.
3. Impose handling on programmers.

If a function can fail, such as `std::stoi`, then sometimes it may not be able to produce a good return value, which suggests that the programmer that wants to consume a value of that type should perhaps build a default that can be supplied to the function in case the function overwrites it.  Then, the function should return something that may be interpreted as a boolean.  This way, the user may choose to deal with the failure if the default value is not good enough.  This solution asks of the programmer to only build a default value.  That is much better than forcing them to deal with exceptions, and their runtime cost.

# Error handler functions, callbacks

Exceptions are good as much as they allow *transactional semantics*, if an error happens, then things can be retried.  But what about trying to make it so that they don't fail in the first place? perhaps the user may know that a problem may happen and have a solution ready if it does.  "The hard disk is close to full? no problem, I am going to give you a function to call me back if the space gets exhausted, I will then free up some space".  The same thing with dynamic or heap memory:  C++ already defines an important callback function, the one supplied to [`std::set_new_handler`](http://en.cppreference.com/w/cpp/memory/new/set_new_handler) for whenever dynamic memory is exhausted.

Because of the template mechanism C++ provides much better support for this way of doing error handling than plain C.  If the invocation chain from the caller to the point where an error occurs are all templates, it is possible that the successive functions convey type information about the original caller, which gives the error handler more context to solve the issue.  A callback can then just fail if it can't solve the issue by raising an exception.

# The monadic approach

I am not setting about explaining what is a *monad* right now.  I liked [Monads in C++](https://bartoszmilewski.com/2011/07/11/monads-in-c/) by Bartosz Milewski.

My audience will surely appreciate a more practical, and C++ centric explanation thanks to Alexei Alexandrescu in his presentation [Systematic Error Handling in C++](https://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C).

The exception mechanism, for all its virtues, suffers from (quoting liberally from Alexandrescu):

1. Slow when they occur, you may want to support the occurrence of errors in cheaper ways
2. Eminently serial:
    1. There can be only one exception being propagating
    2. It needs to be dealt with before any retrying
    3. it requires its own control flow
3. They tend to be an implementation detail not directly associated with the currently executing task or its objective.

Then Alexandrescu proposes that exceptions be error codes, that is, that exceptions are taken care of by wrapping them in a container, and then actual processing of the errors may happen at the program's choice.

Alexandrescu explains that there are very similar things, where `T` is the type of the value expected while calling a function:

1. Haskell `Maybe T`
2. Scala `Option[T]`
3. C# `Nullable<T>`
4. Boost C++ libraries, C++ 17 standard library `optional<T>`
5. I would say that boost and C++ 17 standard librry `any` is relevant, since they also allow holding the exception

And he proposes `Expect<T>`, a template that roughly means "either a T value or the exception that occurred while trying to get it".  That is, Alexandrescu advocates "lifting" from the expected types to `Expected` of the expected types.  Alexandrescu then mentions other quasi-monads in the standard library such as `promise<T>` and `future<T>`.

Dealing with a monad for errors has numerous advantages: They can respect `noexcept` boundaries, can be merged, "saved now and raised later", the error can be associated with the data that led to the error, etc.

With respect of non-obvious advantages of dealing with errors in a monadic way is the composability of the code that uses monads to deal with potential errors.  This is more abstract, better covered by people who may know less about C++ than us but know a lot more of functional programming, as a matter of fact, the inspiration to write these notes come from just having watched Bartosz Milewski's recent presentation "Monads for C++" that I link [at the 47:33 time mark, where he begins to cover `Option`](https://youtu.be/vkcxgagQ4bM?t=47m33s)

# Outroduction

As a leaving remark, I would like to note that a good monad to handle exceptions must apply some technique of type erasure on the exception types, the component we are currently working on, [AnyContainer](https://github.com/thecppzoo/zoo/blob/master/reference/any.md) offers the type erasure capabilities required to support the wrapping of any exception type or value type, also, we are working on a design that I think is original, to allow the definition of user-selected visitations which will allow the complete implementation of monads.

