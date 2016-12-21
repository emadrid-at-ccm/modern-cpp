### Post class 1 notes

In class we compared C's function `qsort` to `std::sort`

The superiority of `std::sort` was demonstrated on two classes of arguments:

1. Error proneness.
2. Performance.

I forgot a couple of things, these notes go slightly beyond to what we covered
in class.

`qsort` receives:

1. a parameter to represent the beginning of the array to be sorted
2. element count
3. element size
4. comparison function.

With regards to what they do, there is no objection.

But there are these defficiencies, from the perspective of the programmer not
immediately concerned with performance:

1. The beginning of the array must be a `void *` because in C the only
way to treat data in a generic way is to treat them as chunks of bytes in
memory.  That is, through their memory location.  The only thing you can do
to a chunk of bytes is to copy them, sometimes the programmer can legitimately
want more than this.  To be elaborated further in a bit.

2. There is no way to guarantee the comparison function refers to elements
of the same type as the elements in the array.  I made this mistake once,
of applying an integer comparison function for an array that in reality had
"longs", my unit, integration tests worked fine, and the code even held in
production for a while until finally the numbers were big enough that integer
comparison was different from "long" comparisons and the program crashed badly.
It took a long production outage and a lot of time to figure out what was wrong.
Please learn from my bug the importance of type consistency.

3. Confusion about the order of count and size as parameters.

4. It only works with arrays.

5. In most practical cases, the compiler knows what is the type of the elements
in the array at the qsort
function call, but the language is so poor that it forces the programmer to
repeat themselves: typically, you have some `int buffer[size]`, why do you have
to write something like
`qsort(buffer, sizeof(buffer)/sizeof(*buffer), sizeof(*buffer), compare_integers)`?
The programmer repeats themselves, with respect to the type of the elements,
three
times: They say the array is of integers, the size is the size of integers, the
comparison is a comparison of integers.  Also, in many practical situations the
size is known to the compiler too at the point of call.  All of these
repetitions are opportunities to "get it wrong", for example,
`qsort(buffer, size, sizeof(buffer), compare_integers)`
If the programmer changes `int buffer[size]` to `int space[size], *buffer = space`
then `qsort(buffer, sizeof(buffer)/sizeof(*buffer), ...)` is wrong.

In `std::sort`:

1. Because the size of the array is not explicit, it is possible to have
`std::sort(container1.begin(), container2.end())`

2. Also, `std::sort(container1.end(), container1.begin())`

However, none of the two are strong problems because for #1 the language has
evolved into adopting the concept of ranges, which has been implementable for a
long time with features already present in the language.  For #2, getting the
order of parameters wrong, we can also solve in many ways.  For example:

    /// Recommended for noobs, extra compile-time guarantees that catch frequent
    /// rookie mistakes
    namespace noob {
        template<typename Iterator> struct begin_t { Iterator b; };
        template<typename Iterator> struct end_t { Iterator e; };

        template<typename Iterator>
        void sort(begin_t<Iterator> b, end_t<Iterator> e) {
            std::sort(b.b, e.e);
        }

        template<typename I> begin_t<I> make_begin(I i) { return begin_t{i}; }
        template<typename I> end_t<I> make_end(I i) { return end_t{i}; }
    }

If the programmer writes

* `noob::sort(c.end(), c.begin());`
* `noob::sort(noob::make_end(c.end()), noob::make_begin(c.begin()));`

It will fail to compile.  As simple as that.

`noob::sort(noob::make_begin(c.end()), noob::make_end(c.begin()));` will
compile but IMO it is rather unlikely this will be an innocent mistake.
In any case, the
underlying problem is an imperfection in the existing STL: the type of a
`begin` iterator is needlessly the same as the `end` iterator.  This has
also been solved in ranges, furthermore, now, we can have infinite,
lazyly evaluated containers.  "Lazy evaluation" is a technical term to be
explored much later.

By the way, the introduction of `end_t`, `make_end` and the rest introduces
absolutely no performance penalty.  In C++ one can introduce types whose
only purpose is to enforce properties at compilation time.

You may not know, but `std::sort` can be called with plain arrays too:
for `int buffer[size]`, one can `std::sort(buffer, buffer + size)`.  And it is
not limited to arrays or vectors, in fact in can use any container, including
*programmer made* containers as long as their iterators are random access.
One example is double ended queues, `std::deque` instantiations.

Because C is a poor language, the programmer can not tell the compiler useful
things.  The result is that for the case of large arrays, sorting using qsort
is twice as slow as std::sort according to my latest benchmarks of this year.

There are many reasons for this performance hit:

1. Instead of using the most efficient way to compare elements, `qsort` must
execute a function call for every comparison.  Even in 2016 that processors
run repetitive code faster than ever before (because of caches, branch
predictors, ...), it is just slower.

2. The implementation can not use the most efficient instructions available
to the processor to move the elements, it must use a run-time supplied parameter

I can craft examples in which the performance disadvantage of `qsort` is
arbitrarily large.  For example, I can think of a plausible case of array
elements that have each their own "cache" memory.  I can express in C++ that if
an array element needs to be swapped or moved, its cache can be invalidated by
a negligible fraction of the cost of copying the whole cache and copy/move only
the rest of the object.

Arguably, you can create in C a function that also takes a "swap" function.

But if you take this approach of creating "flavors" of the same code for
slightly different use cases, you have the problem of having to program an
exponential number of flavors on the number of criteria.

In C++, if I define the `operator<` that's what `std::sort` will use by default,
by default it will use an implementation of swapping that will copy or
move-construct the values; if I supply my own overload to `swap` for those
element types, that's what will be used.  Those are opportunities to make
sorting cheaper, provided by the programmer, which are not intrusive either of
std::sort or the programmer's data.

Imagine you have an array of doubly-linked-list nodes.  If you move a node,
you'd like to update the corresponding link in the predecessor and the successor
nodes.  You can do just that in C++.

In C, because the language does not allow the expression of commonality, the
programmer is forced to write tedious, repetitive, error prone code, that runs
slower unless you do everything from scratch and straying from the "plain
vanilla" cases will quickly force you to reinvent the wheel.

It is ridiculous to talk about "capturing knowledge in the code" with such a
poor language.  If a programmer has been happy all of their life doing this
kind of tedious, ... programming, they will find illusory virtues to their
approach, and be vehement in their uninformed criticism to what I submit as
an objectively superior option.

This does not mean that the improvement in reliability, the economy of
programmer effort and the improvement in performance come for free; to start
with, the compilation errors will be probably inintelligible to the rookie,
the programming tools will be/are clueless with regards to the code, and so on.
