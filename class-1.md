# Modern C++

### Why C++:
1. Performance of large applications, libraries, system-level software
2. Allows to specify things exactly

### Why not?
1. Very hard to learn
	1. Inherent complexity
	1. Accidental complexity
		1. Naked pointers
		1. Generic paradigm is functional, as opposed to normal C++
1. Time consuming
	1. Template errors
	1. Compile, deploy, run
		1. Binary incompatibilities
	1. Baggage:
		1. headers
		2. default mutability
	1. Extremely sensitive to bad practices

#### Nature:
1. Eminently for practitioners
2. I have plenty of experience succeeding at challenges
3. I hope sharing my experience will decimate unnecessary costs:  Oriented towards what's difficult to find in books

### Why yes?
1. Expressivity
2. Reliability
3. Performance

* Supports generic programming
	* Major conceptual shift
* Functional programming
* Object orientation, imperative
* Pattern matching
* Value Semantics
* User-defined data types and algorithms are superior to primitives

* Performance comes from expressing things for the compiler to know more, thus, expressivity is at the root of performance, reliability.

#### Using abstraction mechanisms does not detract from performance but **helps**

* At the same time, learn how to be *less specific* to give room to the compiler to do more

* Meta programming.

### Course: Four chapters:
1. RAII
2. The type system: how to tell things to the compiler
3. Functional programming
4. Using C, Python, others along with C++


#### What is the most powerful feature of C++?
The closing curly bracket.

Error management

Error codes

Exceptions

#### RAII + Exception rules make it practical to be optimistic

Partially constructed objects
