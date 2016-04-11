### Class 3

Resources, sessions, dynamic objects, composition

#### Resources and sessions

During the previous classes we have talked about *resources*, examples:

* RAM Memory -- dynamic memory must be released (given back to the runtime environment)
* files -- must be closed
* threads -- must be joined
* database connections, cursors, prepared queries
* A fictional laser-milling device -- the laser must be turned off
* Mutually exclusive objects (mutexes)
* Condition signals

The concept of a resource is useful to model application-domain things.

For example, an order may temporarily withdraw funds from an account, the order must be finalized, it either gets filled (partially or completely) or gets cancelled.

Associated with resources are the concept of "sessions".

For example, a "lock" on a mutex may be viewed as either a resource or a session on a resource.  The *session* conveys the idea of having a resource committed to some code for the lifetime of its handler.

What is the easiest of resources to handle?

Memory:  it is either assigned to the system or to the execution environment.

The RAII pattern allows the management of resources of *arbitrary complexity*

### Composition

C++ has excellent data composition rules:
1. Allows the definition of memory layouts of arbitrary size, complexity and internal structure
	1. Contiguous
2. Granularity up to the individual bit
3. Composes infinitely
4. Preserves value semantics
5. Provides for generalization/specialization through inheritance
	1. Including multiple inheritance
	2. Virtual inheritance
	3. Provides the choice between
		1. Composition
		2. Inheritance
		3. Referential semantics
6. Generalization/specialization through compile-time templates
	1. Generic programming
	2. Actual specializations, in which the compiler makes different choices when it detects more specific types
7. Allows viewing the same data in different ways (`union`)
8. Allows *variants*, discriminated unions, and the idiom of *type erasure*

#### What is information?

*data* is the model of a fact, *information* is when data produces a change in oneself.
*data* requires *interpretation*.  The interpretation can be the operations applicable to the data.  For example, an integer number, a floating point number, and a complex number.

C++ also has excellent ways to express the interpretations of data.  It is the type system.  Will be studied in the next chapter.

In C++, user defined data has perhaps even better support than data supplied in the standard:
* It is possible to define arbitrary complex data
* There is no limitation on the applicability of new operations to user defined data (it is possible to overload operators involving at least one user-defined type, but not language primitives)
* Inheritance is applicable to user-defined data

C++ allows management of resources by making it so that the creation or acquisition of the resource occurs in the constructor, and the necessary clean up occurs in the destructor.  Data + interpretation.  Data + code.

The Standard C++ makes it very easy to express resources:  Language facilities, standard library components, support for *idioms*.

I do not like talking in terms of encapsulation, transparency, *hiding* implementation details.  I prefer talking of *effective composition*:
* The programmer needs not know implementation details, using a component does the necessary things to use the component without the programmer intervention.  Details of components seldom *leak* to the user programmer interfaces; however, the user programmer *has access* to internals, can benefit from them, can even change them!
* The component programmer has a variety of ways to leverage other components:
	* Using components forces few restrictions on the using component
	* Components can be defined that *force restrictions* as a choice of the component implementer

RAII, completion:

Local variables versus values on the heap.

Dynamic lifetimes:
Referential semantics:
* Because the session associated with the value can not be bound to a scope
* Because the value is shared
* As optimization to prevent unnecessary copying
* To indicate optionality
* To reduce the specificity of the value

All of these use cases imply the concept of *ownership*.  Ownership is who has the responsibility to delete the non-local value:
* exclusive ownership: unique_ptr
* shared ownership: shared_ptr
* ownership transfers: std::move
* non-owning views: normal 'naked' pointers and references, weak_ptr

The programmer must decide on ownership considerations.

Lisp (Scheme) is the most expressive languages of all, the normal pattern is to make code that makes code, thus, it is truly hard to program programs that will generate programs with good choices on the ownership relationships

The ownership relationship is approximated by other languages through the accesibility relationship

Modern C++ is all about being explicit and clear about the ownership relationships.  There is very fine grained sets of choices about how to code the ownership relationship.

Implementation of yasop
