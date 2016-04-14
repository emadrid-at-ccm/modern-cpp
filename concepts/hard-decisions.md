It is difficult to do great programming code in any language, however, there are languages more hostile than others
to unusual requirements.  C++ is very expressive, and will impose very few restrictions on you; this frequently
leads to the dilemma of choice, furthermore, your decisions may be inconsistent and hurt each other, and
to give you choices, there is no good default which forces you to over-decide things.  Other languages take the
decisions for you, and you have to deal with them when they don't suit you.

For example, with regards to managing resources, most modern languages will not give you any automatic support
whatsoever for releasing them in the presence of errors, always in some way or another the user-programmer must
do something or other *explicitly*, in C++ the library author can make it so that using resources is encapsulated
in such a way that language features effectively relieve the user programmer of any concern regarding errors,
the user programmer can program in an "optimistic" way, or concentrate exclusively on the subject at hand, that
the errors that can happen in the components he or she uses may be dealt with in other places of the code that
want to deal with them.  In the case of the resource of memory and its allocations, the support of most
modern programming languages is just the mechanism of tracing garbage collection.  Think about it, if the language
would have good support for the management of resources, then those mechanisms could be used to manage memory and
all the drawbacks of tracing garbage collection would not be necessary.

In C++ one can model memory allocations and actually all kinds of resources in terms of *ownership* relations;
however, the programmer must *decide* on all the choices to express ownership (shared, exclusive, transfers,
non-owning views).  I would say this is one of the hardest decisions programming C++.

Another hard choice is when to use referential semantics (which is common to most modern languages) versus the
rather unique to C++ choice of value semantics.

Another is when to use dynamic polymorphism versus compile-time polymorphism

We will see another set of hard decisions when covering how much to allow extensibility or customization of
generic frameworks.
