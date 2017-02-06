# Quiz 2

#### After 3rd class, [Mark Isaacson's presentation](https://vimeo.com/131194141), first part and template argument deduction

## Necessary part

1. What is an overload?
1. Why does Mark Isaacson hate `fabs`?
2. Does plain C allow overloads?
3. In C++ pattern matching, what is the typical way to make patterns more broad?
4. How are the patterns made more specific?
5. What is your understanding of "more specific overload"?
6. Can the compiler deduce template arguments to a template function call?
7. Is it possible to specify some template arguments and have the compiler deduce other template arguments within the same function call?
8. Is the order of template arguments a matter of style?
9. Do automatic conversions apply when deducing template parameters?
10. What is type dispatch?
11. What are type traits?
12. What is the hierarchy of iterator categories? (you do not need to know the hierarchy itself, just what it is)

## Optional part

1. Identify two errors in the presentation slides.
    1. By the way, in class we identified *three* errors, one was a *design* error.
3. What does `const auto &` mean?
2. Why do the original implementations of `getNthElement` return concrete types but later they return `const auto &`?
4. From the part watched, please indicate instances of the concepts of:
    1. Cohesion
    2. Coupling (or lack of coupling)
    3. Orthogonality
5. "Explicit typing" must be avoided almost at all costs, but what is explicit typing?
    1. Why must explicit typing be avoided?
6. Why is it foolish to insist on using `memcpy`, `memmove`, and the such? (of course, except to wrap them to prevent user code to use them!)
    1. Hint, go over all the work we did comparing the plain-C style `qsort` to `std::sort`
7. How can automatic conversions be forced in a template function call?
    - **Note:** If there is `struct Base` and `struct Derived: Base` (that is `Derived` inherits from `Base`), and there is function `void f(Base)`, if `d` is of type `Derived`, the call `f(d)` is not an automatic conversion from `Derived` to `Base`.
8. Around the mark 31:40 in the presentation, the taxonomy of iterators is presented.  That is a very fundamental taxonomy.  Can you think of examples of taxonomies like this?
    1. Are pure-type taxonomies such as this representable in other languages of your liking? or are these taxonomies represented as classes with specific operations, so they are not pure types? The goal is to observe that in C++ among very few languages it is possible to enrich types *outside* the type.
    2. Do these taxonomies and using them create a performance disadvantage?
    3. **Note:** I think the iterator taxonomy is a truly great example of how to *encode knowledge*, from that point on, vast amounts of operations on containers can be implemented generically to achieve higher software quality, save programmer effort, and get higher performance, **all three at the same time**.  Please attempt to write a template function `mysort` that will call `std::sort` if the container iterator is random access, or your own sort function that does not require random access iterators.
        1. Implement a non-random access iterator quicksort.  You may assume there exists a function `swap`, you are forbidden to create temporaries of container elements (this is not an extra challenge, but an essential characteristic of any container algorithm), iterators, on the other hand, are assumed to have *value semantics*, and that it is cheap to copy them, have collections of them (quicksort has non-constant space complexity).  This is an extremely efficient exercise in terms of learning for the effort IMO.  Estimate about 30 minutes.
