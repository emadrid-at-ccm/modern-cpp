### Ownership

With regards to dynamic objects, that is, the instances whose lifetimes are not
bound to a lexical scope but controlled by the applications themselves, I think
it is useful to think in terms of what component or piece of software within the
application has the *reponsibility* to deallocate them.  At the same time, since
it is so easy to model resources through handler classes in such a way that the
lifetime of the instance is the *session* in which the resource is held, all the
things that relate to deallocation of dynamic objects also relate to resource
management.

*Ownership* is participating in the responsibility to destroy a dynamic object.

Dynamic objects can be shared, by using their address (including the C++
references) *without* sharing ownership.

Ownership can be *exclusive*, only one thing has the responsibility to destroy.

It can be *shared*, meaning that several software elements may potentially
have to destroy the dynamic object.  Of course dynamic objects are destroyed
only once, but there may be several software element that may activate the
destruction.

Also, ownership can be *transferred*.  An example is moving from an `unique_ptr`
into another: the "moved from" `unique_ptr` has no reponsibility to destroy the
dynamic object, it is all up to the "moved to" `unique_ptr`.

### Delayed destruction or collection

When a `shared_ptr` is 'reset', the underlying mechanims is for the shared_ptr
to indicate to the resource it won't be sharing its ownership, if there are no
more `shared_ptr` on the same object, then the last to participate in the
ownership is the sole owner, and that is the point of destruction.  This may be
a performance disadvantage; if the resources associated with the dynamic object
can be held longer, the ownership can be `transferred` to a collector software
component.

In languages that have tracing garbage collection, in this terminology, all
references are sharing ownership references, then the relationship of
*accessibility* and *onwership* are the same; however, the language runtime
can not know when a dynamic object does not have any more valid owners, for that
it must traverse the whole accessibility graph at some unespecified later time,
the garbage collection stages, cycles.

By allowing the expression of non-owning references, and fine grained choices
for ownership, **in C++ it is possible to routinely achive performance in the
management of resources that can only be obtained with ad-hoc resource
management, but while at the same time getting varying degrees of reliability
and economy of effort**.

The choices about the ownership considerations regarding data relationships is
one area that leads to very different levels of software quality depending on
how "right" the programmer gets them.  Considering that sometimes data models
are produced by *programs*, the simplest form of ownership, shared, is possibly
the only practical choice.  Beause C++ does not yet have introspection
capabilities, it is in general not possible to construct the accessibility graph
of objects, thus, the only technique available for fully automatic expression
of data relationships is through shared ownership references such as
`shared_ptr` typically implemented with reference counting mechanisms, but
reference counting garbage collection significantly underperforms *tracing
garbage collection*.  As an aside, this is an important factor that makes
languages such as Lisp and Javascript superior for higher-order programs
(programs that make programs), the simplicity of the syntax for the program
code itself and the tracing garbage collection for the part of ownership in the
data relationships.

I have only limited experience palliating the lack of introspection in C++, but
it seems depending of how practically the instrospection issue is solved, the
choice of tracing garbage collection becomes viable.
