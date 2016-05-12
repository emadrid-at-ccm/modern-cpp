### The Type System, continued

#### The need to be mathematically minded towards the type operations

1. Ranged iteration
	1. Allows paying runtime performance to be defensive detecting errors
	2. Conveys more information to everybody
2. Type operations began imitating other languages, concepts, before discoveries in C++, then became a frequent source of accidental complexity.  Examples:
	1. Pointer versus iterator
	2. Unresolved dichotomy of std::string between container and value
		1. Value semantics, referential semantics
			1. Aggregation in Java
3. Inheritance: built-in can't be inherited from
4. Introduction to the overloading rules:
	1. Exact match
	2. Implicit conversions
	3. Templates
		1. Substitution
		2. SFINAE
	4. void *
5. Smart pointers should resemble plain pointers: std::dynamic_pointer_cast
6. Difference between function templates and class templates
	1. Automatic type deduction
	2. Partial Specialization
	3. Why it is disadvised to (fully) specialize function templates

#### CRTP
1. Intrusive versus non-intrusive mechanisms
	- Java arrays
	- Multiple inheritance
	- Multiple inheritance of implementations
	- CRTP
	- Bjarne Stroustrup before Alexander Stepanov invented the precursor of the STL
2. YASOP:  Yet Another Shared Ownership Pointer
	- Problem common to all smart pointers: the unnecessary destructor expense!
	- Undead instances
		- Unresolved semantics
	- Problem of shared_ptr
	- boost::intrusive_ptr
		- Why boost::intrusive_ptr is intrusive
		- Sean Parent's presentation "Inheritance is the base class of evil"
	- YASOP

#### Duck typing

- Before C++11 C++ was 100% *imperative* with regards to types, the experience of using `auto` since C++11 led to strongly recommend to everybody to use automatically deduced types.
- Instead of telling the compiler what things are, don't even tell it what they do, let it figure it out.
- The expression "Duck Typing" became most popular thanks to Python.
	- Evident performance cost
- If you care about something that walks like a duck, swims like a duck, and quacks like a duck, do you care whether the thing actually belongs to the family Anatidae?
	- Even then, it may not be a Duck.
	- That is the problem with imperative typing
- It can be expressed in modern C++ that something does not care about being Anatidae as long as it has the duck attributes of interest.
- The fatal flaw of duck typing.

#### Pattern matching
The rules of the language allows the expression of patterns of types for the compiler to choose among competing options.  The fundamental mechanism is overload.  Overloading is at the heart of the support for duck typing, that is, it allows the expression of the desired behavior.

#### Overloading

1. Implicit conversions
	- Includes derived pointer/reference to base pointer/reference
	- Type promotions
	- Array decay into pointers
	- Constructors and conversion operators
2. Koenig lookup
3. Overloading with templates
	- Automatic template argument deduction, not automatic invention!
4. Type token arguments

## Templates
1. The substitution mechanism (unification)
2. The overload set
3. Expressing patterns through substitution failure (SFINAE)

Presentation by Mark Isaacson
