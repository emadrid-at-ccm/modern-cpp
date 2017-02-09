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
    3. Implementation details change the public interface, exposes user code to the implementation details
    4. 
    4. For example, in Maystreet's Bellport the error codes became a kitchen sink of everything that could fail in the API.
