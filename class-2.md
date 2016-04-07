## Class #2

We made sure to have a sufficiently good understanding of what was a "resource"

The subject of the lifetime of local objects was covered

The order of creation and destruction of class member variables was covered

The subject of exception propagation while partially creating objects was
covered.

These rules can be summarized with:  When the program execution leaves a scope
(block of code, initializer list, etc) all of the variables fully constructed
will be destroyed.

This subject is IMO extremely important because many awesome techniques that at
the same time reduce programming effort, give performance and reliability
can only be used effectively if the programmer has very clear understanding of
the lifetime of objects.

In particular, if the resources are modelled through handlers in which the
resources are acquired in the constructor of the handler and released in the
destructor, then knowing the lifetime of objects

* guarantees knowing when the resources are released
* that there are no resource leaks
* allows to program optimistically (without any extra effort the rules of the
language will release resources automatically in the presence of exceptions,
thus, code not interested in dealing with errors can *simply and safely ignore
them!*, deferring to code actually interested in dealing with the errors)

Bjarne Stroustrup invented the term and acronym "RAII", which stands for
"Resource Acquisition Is Initialization", and means what was just explained
above.

Furthermore, the "handlers" technique, RAII, allows very straightforward code
reuse and code generalization, both generalization in the "Object Oriented"
way and "Generic Programming" way.  These benefits will be explored later in the
course.

To complete this aspect we will start next class with the rules for the lifetime
of base classes, considerations about dynamic polymorphic classes (that is,
classes with `virtual` member functions or `virtual` base classes).

We will also go over techniques to program inherently "retriable" code and fully
achieve optimistic programming.  At the core of these techniques are a
*transactional* attitude towards changing the *global state* of the running
program or system which is good in its own right.  We will talk about preventing
irreversible changes.  This is a contributor factor for the raise in popularity
of the functional programming paradigm that among its fundamental tenets tries
to avoid chaning the state of execution, an area particularly active of C++
development of libraries, *idioms*, and new language features to better support
them.

#### Class 1 code

#### Comparison between sorting in C++ and C

This example illustrates motifs that recurr very frequently in C++, motifs that
I will be making a lot of emphasis on; it is about how C++ features work
together to provide unbeatable performance, reliability and economy of effort
all three at the same time.
The comparison is against C because C has the reputation of being the language
to get the highest performance possible in practice.  If I were to compare
against Java, Python, etc; on top of most of the elements of the comparison
being true, the performance superiority of C++ would be dramatic.  In a sense,
comparing C++ style sort to C sorting is the thoughest.

	void qsort
	(
		void *array_beginning, unsigned element_count, unsigned element_size,
		int (*element_comparison)(const void *left, const void *right)
	);


	template<
		typename RAI, // RAI stands for "Random Access Iterator"
		typename ComparisonCallable 
	> 
	void sort(RAI begin, RAI past_end, ComparisonCallable cmp);


Note: the participant must understand:

1. What is a declaration
1. What is a definition
1. What is a pointer
2. What is a const pointer
3. What is a void pointer
4. That all pointers are convertible to `const void *`
5. Converting a `void *` to a pointer to some other type is called "casting"
the pointer
16. Convertibility of pointers
6. What is a template argument
7. What is a type argument to a template
8. What is an iterator
9. What is a "callable" or function object or "functor"
10. What is overloading
11. What is operator overloading
12. What is template instantiation, or template parameter substitution
13. What is an array
14. What is inlining
15. What is linking

Note2: I am attempting for explanation purposes to make qsort and sort as
similar as possible, in normal C++, std::sort defaults the comparison to
"less than".

An example of their usage, inside a function, there is an array, declared and
defined as follows:

	int values[] = { 7, 9, 23, 72, 5, 1, 8 };

In the case of C++, to sort such an array, all it takes is the following:

	std::sort(values, values + sizeof(values)/sizeof(value[0]), std::less<int>());

Of course both C and C++ lets you capture the count of elements:

	int count = sizeof(values)/sizeof(value[0]);

To use the C library sorting function, it is necessary to have a comparison
function:

	int cmpInt(const void *l, const void *r)
	{
		const int
			*lip = (const int *)l,
			*rip = (const int *)r;
		return *lip - *rip;
	}

Then we can call the following:

	qsort(values, count, sizeof(int), cmpInt);

So far, both things represent similar amount of work.  If all you ever want to
sort are integers, probably the C solution will suffice.

IMO C++ is best applied for large and performance conscious systems, in there,
sorting will be used in variety of forms.

If instead of a primitive array, one has a "managed array", in C++ an
std::vector; one can have:

	std::vector<int> values = { 7, 9, 23, 72, 5, 1, 8 };

Observe that basically there is no extra effort involved to use managed arrays.
To sort:

	std::sort(values.begin(), values.end(), std::less<int>());

In C++, as a matter of course, the very same code works regardless of the
following:

* Whether one is using a primitive array, a compile time array, a managed
array, a double ended queue, or any container, including those made made by the
programmer as long as the iterator in the container is "random access", that is,
it allows to "jump"
* Intuitively there is no limitation on the type of the element in the array
as long as it makes sense to keep them in arrays and to sort them
* In `qsort`, there are many hard-assumptions, that is, the code will not work
if the arguments or types involved don't satisfy them, including:
	1. The elements are arranged in a contiguos memory array
	2. Swapping an element (to put it in the right place) is exactly to swap the
	all of the raw memory of the elements --nothing more *and nothing less too*
* It is easy to make mistakes with `qsort`, for example, yours truly once had
this bug:  The array of elements was changed from integers to `long`.  I changed
the other parameters except the comparison function.  It compiled.  It seemed to
work just fine.  Months later, it had catastrophic crashes.  **That is, the
enforcement of the parameters to agree with each other is not helped by the
language nor the compiler**
* In `qsort`, the code resides in a binary library, *object code* already
compiled and ready to be used as - is
* In `std::sort` the compiler will use the template code and create optimal
machine code.  This has many consequences, including:
	1. Longer compilation times
	2. The opportunity, which is used in practice, for the compiler to
	**guarantee the parameters make sense together**, sometimes, there is even
	the opportunity to run unit-tests *while compiling!*
	3. It is trivial to override the default mechanism to swap elements to any
	arbitrary code one wants
	4. The compiler not only inlines the swapping and comparison functions,
	which tend to be 10 times faster than going through the motions of a normal
	function call, but will additionally "know" for example, whether the
	elements fit into a processor register and thus can be juggled without
	using memory; can decide the optimal point (based off the cost of
	comparisons, the size of the elements, etc) whether to switch from normal
	"quicksort" sorting into "insertion sorting"
	5. Exactly the same interface can be adapted to sort in different ways, for
	example, using the "merge sort" algorithm for containers that don't have
	random access iterators (that is, the iterators can't "jump")
* The occurrence of an error, for example, the process running out of temporary
memory to perform the sorting, is a fatal error in qsort, whereas it may be
a trivially recoverable error in C++

Remember, C++ is a superior language, it truly delivers on its promises, just
that one must be a good programmer to appreciate their value, and one must
learn quite a lot.  Most programmers know only about their application domain,
they don't care about the benefits of C++ features; and they definitively don't
bother to learn C++.

### Aperitif:

A function that will sort containers and primitive arrays just the same:

[See the compiler explorer output](http://gcc.godbolt.org/#compilers:!((compiler:g492,options:'-O3+-std%3Dc%2B%2B11',sourcez:MQSwdgxgNgrgJgUwAQB4CGUDmB7ATiAFwAsBbAPgChRJZFUA3BCAvSighEgByjQ5QIBPLgjBoSyACpkk9bCDhIAzngIAKSUgBkEbGAJpwCXAEokAbwpJrygnABc9lbnW79hsMYB0AIwSZwNRMAGiQ3AyNcL1E4IJMAbgoAXwp2Th4%2BBAFhUXEpUJgwJRBMT0UAGRk5BWVVDSQ1LTRcUwBtcoBdM0sbWwcnOubcUKGkAGokcoTk1OrFZwJ6WKU7R0ZmPBQ4bBgfKAQZLSXu2pc1Y/ikFIpwAiRMKGwfDAB9IbRBVo6kAF4LJAAHKEAJyhACMAAZQgB2UIAFiuiQoc1OBBeDyeGCC/wWagxzygbxaHwSV1S1Gg8GQKBWcCgIB8XiIbFuYW4ajcK1k8kUACooKFOXcUbzTP9cAgCDBcGAkLy1KzeSYoEgALRyhX6OUmXCXa4ohYvACOuJOJrq%2BNe70EoQArPDBdxSdcgAAA)),filterAsm:(colouriseAsm:!t,commentOnly:!t,directives:!t,labels:!t),version:3)

	#include <algorithm>
	#include <vector>

	template<typename T> void sort(T &container) {
	    std::sort(container.begin(), container.end());
	}

	template<typename T, unsigned L> void sort(T (&arr)[L]) {
	    std::sort(arr, arr + L);
	}

	void sortvd(std::vector<double> &vd) { sort(vd); }

	int global_array[] = { 8, 9, 10, 7, 4 };

	void sort_global() { sort(global_array); }

	#include <stdlib.h>

	int cmp(const void *l, const void *r) { return *(int *)l - *(int *)r; }

	void sort_qsort() { qsort(global_array, 5, 4, cmp); }


