# How to handle errors

The subject of errors is of capital importance, in my experience not doing this right is what leads to the vast majority of bugs and wasted programmer efforts.

C++ is particularly good at handling errors.  It gives three very reasonable options, and it also supports error codes.

I had said there were three ways to handle errors, it turns out there are four, I was not fully aware of the monadic way.

Let us cover the four ways:

# Error codes

This is the worst of all possible options, however, it is the most prevalent :disappointed: perhaps because a very poor language like C can only support this way.

Error codes don't really have advantages, but it is important to understand the deficiencies that programmers ignore to their detriment.  Quoting [one of my lecture notes](https://github.com/emadrid-at-ccm/modern-cpp/blob/48c012556d53be7e2770c1e9996586b01ebea786/3.5/Class5-post.md#error-codes), and improving the explanations *in italics*:

# Error Codes

0. They require the programmer to consistently check for each function that may fail:
    1. Never done consistently in practice
    2. Introduces a performance penalty, the check itself
    3. Uglyfies the code
    4. Impossible to audit in practice *because error codes are arbitrary codes, in general, the compiler does not know which are the codes a function can return*
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
2. Committing execution environment changes only when the transactions have completed.

With these two things, code that fails due to an exception being raised, won't damage the execution environment, and should be retriable, for no extra cost whatsoever to the programmer.

Words of practical advice:

*. If your code wants to write a record to a file, please make sure it only writes complete records.  Keep a structure with the record partially formed as opposed to write the record partially to file; that way you benefit from the *transactional semantics* allowed by the excellent support for exceptions in C++: If something fails, your file does not get corrupted, you may try writing the record at a later time.

This is an interesting topic, I invite the reader to consider these things:

1. C++ does not have a `finally` clause because it does not need it.  `finally` is useful in other languages like Java for cleaning up resources if exceptions happen, the code that gets executed after the `try` regardless.  Proper C++ cleans after itself in the destructors, the destructors are called even when unwinding the stack to propagate exceptions.
2. Currently C++ has what is called "zero cost" support for exceptions, which means that at runtime there is no thunking or framing of the stack to support exceptions, the frames are specified at compilation time, the exception mechanism searches based on the "Program Counter" or "Instruction Pointer" the frame where it is, that frame contains program code to deal with the exception.  This does not mean that exceptions, when they happen are "free of cost", just that *supporting* exceptions adds only the exception frame data to the object code, without any further runtime cost.  This is explained in [The true cost of zero cost exceptions](https://mortoray.com/2013/09/12/the-true-cost-of-zero-cost-exceptions/).
3. There are complications, such as that destructors should not raise exceptions, since destructors are invoked from within exception propagations, supporting destructors that my throw may require the support of the propagation of more than one exception.  In C++ **raising an exception from within the context of an exception being thrown is program termination**.  This is the second of the reasons why destructors are the most important pieces of code (the other being that since they are always called, getting them right is essential for performance reasons).
4. There is a problem with exceptions in general, which 




[Bartosz Milewski](https://youtu.be/vkcxgagQ4bM?t=47m33s)
