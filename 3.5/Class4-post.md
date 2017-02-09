We reviewed [quiz 2](quiz-2.md), including the optional part.

We set to build a quick sort algorithm dropping the requirement of random access.  We got code similar
to [sort.cpp](../code/sort.cpp)

We ran the benchmark, and saw this code performs **faster** than C library's `qsort`.  This is an illustration of what *Generic Programming* is: Reasoning in an abstract way we find the way to make high-performing code which is applicable to *vast* number of cases not covered by plain vanilla solutions.  In particular, this implementation of sorting works with *any* container, provided the elements are sufficiently random so that this quicksort does not degenerate into quadratic sorting.

Our non-random-access implementation has several performance disadvantages compared to `qsort`, including, potentially:

1. It is the quicksort algorithm all the way through, which is not efficient since doing quadratic sorting for small number of elements is faster
2. It has no optimization tricks whatsoever, such as using temporary buffers to store elements.
3. In a performance-critical part, the partitioning, it emulates a 3-element rotation using two swaps.

The interested student may perform several worthy experiments, for example:

0. Count the comparisons in `std::sort`, for example, sorting a container whose elements compare incrementing a counter, the same with swaps or moves
1. Invoke `std::sort` with a container whose comparison operator is "opaque" to the compiler, and measure the relative performance hit.
2. Invoke `std::sort` with a container whose element move or swap operations in an opaque way to the compiler, measure the performance hit.
3. Do a type-dispatch `mysort` function that will call `std::sort` or this non-random access quicksort.
4. I have a disatisfaction with the funciton we wrote, it swaps elements, even when the elements live in a container such as
`std::list` where we have the freedom to rearrange the *container itself* as opposed to the elements.  This implementation of
sort would be very inefficient for lists of elements that are very expensive to copy, because instead of relinking the list, which must be inexpensive, the elements are swapped.  Before putting it into code, reason about the following questions informally, with the intention to later learn how to encode these things into C++:
    1. How would you characterize "elements that are expensive to copy"
    2. How would you characterize "containers in which the program code can easily, or inexpensively, change the order of the
    elements leaving the elements fixed".
    3. You already know the fundamental mechanism to select among competing implementations, assume you can have traits or
    some compilation time mechanism to accomplish 1. and 2., and then make the code that will dispatch to optimal sort
    implementation functions.
    4. How do you characterize elements that can be copied to a temporary buffer? This is important because we can accelerate sorting by copying elements to a temporary buffer.  By the way, there are many standard template library containers that do not require their elements to be copiable to temporaries, in which once you put an element in, the element never changes position.  This leads to hopefully interesting to you questions:
        1. What should happen if the user has a container such as `vector` that is suitable for algorithms such as `std::sort` but that the programmers can instantiate with non-moveable elements, legitimately, and the user by mistake attempts to sort using `std::sort`?.
        2. Why would a programmer want to use non-moveable elements?
        3. A container such as `std::list` clearly does not require elements to be moved for operations that change the container such as adding elements, removing them, even sorting.  How is non-moveability accomplished with containers such as `std::vector`? (what should the programmer do?).  Use your intuition.  Good generic pieces of software do not require documentation to be used, the operations that should be present for the most part do exist and do what we expect, but generic programming require *good programmers* to take advantage of what it offers, that is, programmers with this kind of intuition well developed.
    5. Can you do any of this, in a practical way, with classical object orientation?
5. Make the sorting function faster, by implementing improvements that deal with the potential performance disadvantages 1,...,3 mentioned before.
6. Make a sort algorithm which continues to be quicksort in essence but that does not degenerate into quadratic sorting if the input is already sorted.
7. Paul's question, do pseudo code for a non-random access assumption quicksort which leverages parallelism.
    1. Perhaps you should start with a simpler algorithm:  To accumulate all the elements in a large container using parallelism, and the accumulation can be done *destructively*, that is, the vector may be left with whatever contents after accumulation.
        1. What is the time complexity of that algorithm?
    2. It is clear the recursion step can be done in parallel.
        1. What about the partitioning?
        2. What if the algorithm requires an hybridization between quicksort and a merging phase?

These questions are typical in nature once you get used to Generic Programming.  The characterization exercises lead to ever better *concepts*; great concepts lead to great code: One can implement a single function that is usable in wildly different cases, such as our non-random access sorting, confident that it will do the right thing every time.  That allows us to think of improvements that automatically apply to very many cases.  It also helps us to recruit the help of users into providing truly minimal guarantees:  You are identifying the properties that are *orthogonal* to the others, providing explicit support or requirements for them.
