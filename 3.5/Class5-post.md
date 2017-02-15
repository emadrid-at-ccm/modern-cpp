This class almost completes the notes for [class 2](./Class2.md)

We covered how to handle errors.

There are three ways to handle errors:

1. Through error codes
2. Through exceptions
3. Through callbacks

# Error Codes

0. They require the programmer to consistently check for each function that may fail:
    1. Never done consistently in practice
    2. Introduces a performance penalty, the check itself
    3. Uglyfies the code
    4. Impossible to audit in practice
1. Error codes are just a code or a label for the problem that happened.
    1. The information that describes the error belongs to the execution context
        1. Presents complications to make the information related to the error thread safe
        2. It is global to the process, ephemeral: The error information must be checked immediately before calling anything that could overwrite it if there are further errors.
2. It is a brittle mechanism: If some function changes and may have a new error code, what is the guarantee all the callers, and the callers of the callers that do not "swallow" the error will correctly deal with the new code?
3. Error codes are notoriously hard to propagate:
    1. The error codes for a funtion becomes the union of the error codes of all of the things that can fail within them
    2. Frequently, two different functions use the same code for different errors, so, error codes need conversion functions
    3. Because the codes themselves are part of the user interface, implementation details change the public interface, exposes user code to the implementation details
    4. For example, in Maystreet's Bellport the error codes became a kitchen sink of everything that could fail in the API.
4. Digression: Most languages implicity define optional semantics using the equivalent of an error code:  Return a `null` reference, `nil`, `none`, or a null pointer.  Don't do this in C++ unless it is really useful, and for that better use [`boost::optional`](http://www.boost.org/doc/libs/1_63_0/libs/optional/doc/html/index.html) or the upcoming [`std::optional`](http://en.cppreference.com/w/cpp/utility/optional)

# Exceptions

We went over the C++ rules for exceptions.  In particular, the exceptions while initializing structure or class members.  The general rule is intuitive: Upon exception, fully initialized members are destroyed in inverse order of construction (LIFO)

It is assumed the participant has very good knowledge of exceptions.  However, the participant may not have considered these things:

1. Why does C++ not have the equivalent of a `finally` clause?
    1. What is `finally`?
    2. What is it good for?
    3. [In Python](https://docs.python.org/2.5/whatsnew/pep-341.html)
2. What is the performance cost of supporting exceptions?
    1. What should exceptions do? What is the propagation of exceptions?
    1. What does "zero-cost" mean?
3. Nested throws are program termination
    1. Destructors must not throw!
    2. There may be legitimate programming techniques that throw on destruction, as anything in C++: you have to really know what you are doing
4. Digression: What is the most powerful feature of C++?
    1. The most important type of code in all of C++ are destructors.  Why?
5. Exceptions are inherently serial
6. They are unrelated to the goal of the code that triggered it, they are an indication of a broken property

# Callbacks

To handle errors, the execution environment may define some way in which some error handling function will be called.  There is one important such error callback function in C++: [`std::set_new_handler`](http://en.cppreference.com/w/cpp/memory/new/set_new_handler).

# Objective:

The objective is to program in a way in which you **ALLOW YOUR PROGRAMMER USERS TO BE OPTIMISTIC**.  What do I mean by this?

1. Errors can always happen (out of disk space, etc).  Optimism is not asking users to deny the reality that errors can happen, but that should an error happens:
    1. Just making the user code retriable is a perfectly sufficient way to handle any error:  If the things the user called succeeded, and the user code itself succeeded, then it is allowed to make an irreversible change.  If there is an error somewhere, and the code was written with this approach in mind, then no irreversible thing happened, the error will continue to propagate until there is a piece of code interested in handling it explicitly.
    2. Making things retriable is desirable by itself
        2. Do not change global state gratuitiously
        3. Functional programming is in essence taking this approach to the ultimate consequences
2. Code is not forced to be polluted with explicit error handling
3. The user concentrates on their goal as opposed to the errors that can happen
