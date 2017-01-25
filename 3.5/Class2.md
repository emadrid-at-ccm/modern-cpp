In the reposition for the first class, on the subject of the deficiencies of
languages or the `qsort` example, one participant quipped workarounds to deal
with them, but they were not true solutions.  It is important as programmers to
be able to adapt the libraries we have to our goals.  Most of the time we don't
have the opportunity to get or make the library that truly fits our purpose
best.  However, this course is not about the "tactical" solutions to get things
done, but about how to encode in programming code our knowledge.  Much later in
the course, toward the end, we will study in systematic ways how to adapt
to restrictions imposed from the outside.  Expressiveness also helps with
adaptability, but first we must learn how the right code looks like, to make
adapters that make the code we can use look as if it was the right code.

Last class we talked about `std::sort` and `qsort`.

I hope to have shown convincingly the multiple ways in which `std::sort` is
objectively superior.

How was this achieved?

How much more work over `qsort` one has to do to obtain the benefits of
`std::sort?` could it be *less*?

1. `std::sort` at the same time cares and does not care about the specific
types of its arguments, the given iterators:  It does not care about their types
as long as they have some properties.  From these properties some things
are deduced, such as how to move/copy elements, how to compare them.
    1. `qsort`: Four arguments in three disjoint groups (only the array pointer
    and the element count work cohesively)
    2. `std::sort`: Two or three arguments with a lot of cohesion, however,
    there is a missed opportunity for cohesion between the beginning and end of
    the range in the form of indicating they belong to the same container.  This
    leads to subtle issues.  An example that at the same time illustrates the
    benefits of cohesion and the costs of its lack.
2. `qsort` makes the strong assumption of an array.  Can't be used with anything
else.  It is **coupled** to the container being an array.  Is this assumption
necessary?
3. `std::sort` uses the following from the iterators:
    1. How to swap, move or copy elements (including their size and side
    effects)
    2. How to compare them (including potential side effects)
    3. How to jump from one place in the container to another

`qsort` relies, I think, on the most complicated data structure directly
supported by C, the array.

`std::sort` is superior because it generalizes every bit in `qsort`.  It relies
on more fundamental concepts:

- A Container - not just an array
- A container element - not just a bunch of bytes of a given size
- Referential semantics - not just a pointer, but iterators
- Container, *referential semantics*: traversal
- Ranges

Not yet the right time to talk about **value semantics** versus **referential
semantics**

## Truly remarkable unique properties of C++ generic programming:

1. User defined types are as powerful to use and as highly performing (even
*more* performing) than primitive types in other languages
2. The abstractions are *not intrusive*, especially do not require *intrusive
runtime mechanisms*
3. The concepts (such as objects, containers, iterators, ranges) are remarkably
synergistic.  Why do they work? -- Based on mathematical *practice* rather than
theory IMO. Great software can be accomplished if the programmers "hit it" with
the concepts they model.
    1. Reason to learn how to identify them, refine them
    2. IMO it is an attitude
    3. Initially one gets it mostly wrong, leading to very ugly, impractical
    code.  Keep practicing until the end of time.  I promise it pays off in
    multiple ways:
        1. You begin to think deeper and deeper about your application domain,
        to decompose it into better concepts
        2. The increased scrutiny of concepts help you learn both application
        domain and programming
        3. It is an analytical approach that makes you more intelligent
            1. It is susceptible of objective comparisons
            2. It is qualitatively noticeable
        4. Empowers you to deal with inherent complexity
        5. It is inherent complexity.  It takes time and effort.
        6. I claim it is very efficient use of your time and efforts.
        7. Alexander Stepanov confesses he had to try several times
    4. Similar to mathematics
        1. Stepanov delirium that led him to feel parallel addition is only
        possible thanks to addition being associative: Parallelism depends on
        the semigroup algebraic structures.  Algebraic structures give rise to
        the concepts, the concepts to great software.
4. [Stepanov complaints against Object Orientation](http://www.stlport.org/resources/StepanovUSA.html),
just mentioning them at this point:
    1. If everything is an object, you are saying nothing.
    2. Interfaces that vary on a single type: leads to the unsolvable problem of
    categorization
    3. The failure of categorization leads to increasingly bizarre busy work
    workarounds like design patterns such as the visitor, you are moving further
    away from the application domain into Object Orientation.
    4. `max` can simply not be used in Object Orientation
    5. It naturally leads to the mindset of "pining for garbage collector" and
    modelling in terms of "boxed, heap-allocated objects" that in my
    nomenclature I refer to "intrusive run time mechanisms" and unavoidable
    "referential semantics", that is, abstractions that are fundamentally false
    to the computer and hard to reason about.
    6. Beyond Stepanov, implicitly, there is the problem of mutable state:
        1. Impossible to reason about
        2. Impossible to parallelize
        3. Also otherwise performance hostile
        5. Not reusable
        4. Not necessary
        5. Further proof that hard object oriented languages, especially those
        without any value semantics, such as Java are inherently slow and
        unreliable
    7. Beyond Stepanov, part 2: "You wanted the banana but what you got was a
    gorilla holding the banana and the entire jungle", the famous quote by Joe
    Armstrong, designer of Erlang, from [Coders At Work](https://www.amazon.com/gp/product/1430219483?ie=UTF8&tag=theende-20&linkCode=xm2&camp=1789&creativeASIN=1430219483), in Object Orientation, the objects have implicitly all of this environment. 
    8. Beyond Stepanov, part 3: Encapsulation is not very good either: The data
    members sometimes are really not cohesive except they are an implementation
    detail, if implementation details can be kept separate, they are not
    encapsulated by definition.  I advocate using global functions whenever
    possible, that leads to possiblities for reuse.  Member functions are
    *coupled* to the class...
    9. Beyond Stepanov, part 4: Object orientation *hides* state, it has all the
    drawbacks of being state-based, and the additional hiding it.
    10. Inheritance: To begin with, most OO programming languages do not support
    multiple inheritance of implementations (Java included, Python to a degree),
    but even if they support multiple inheritance of implementations, like C++
    you quickly hit the unsolvable problem of covariant properties, parallel
    hierarchies.
    11. That is:  Object Orientation is false in every claim.


The most fundamental concept in C++ is the lifetime of an object.

Objects are not just a bunch of bytes of a given size.  They have an
*invariant*, a *construction* and a *destruction*.  Construction and destruction
define the *lifetime*.

## Resources

What is a resource?

1. Memory
2. Files
3. Threads
4. Network connections
5. Database connections
6. Database cursors
7. Peripheral devices

- Acquisition
- Release
- Contention
- Composition and relationships between resources

Modeling resources in software leads to the following challenges:

1. Acquisition
    1. Representing *Ownership*
    2. Representing *Transfer*
2. Release
    1. Does it occur in a *Deterministic* way?
    2. Is it *Certain* it occurs?
    3. Whose *Responsibility* is it?
3. Contention
    1. Performance - thin, true to hardware, operating system abstractions
    2. Synchronization - beyond our scope
4. Composition
    1. How do your known languages support composition?
        1. Referential semantics/value semantics
    2. Generalization/specialization, orthogonality, covariance
    3. Sharing
    4. Belonging

Is Linux object oriented?

## Resource Handlers

If you want to make sure something happens in C++, wrap it as a resource handler
and define the lifetime of the handler in such a way that whatever you need to
happen happens at the end of the lifetime of the handler.

## Resource Acquisition Is Initialization

## Error management

Options: 

1. Error codes
2. Exceptions
3. If not exceptions, what else?

## The most important pieces of code in all of computing:  Destructors.

1. Double throw.
2. Inevitability
3. Hidden
4. Bring dependencies
5. Importance of virtual destructors (runtime polymorphic)
6. Stupidities like OnixS making destructors virtual and protected
