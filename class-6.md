#### Class 6

We covered the subject of smart pointers in depth, including smart pointers as
an example of fine-grained management of resources.

One cultural element of C++ is the skepticism towards "one-true-solutions",
for example, in the case of smart pointers, the standard library defines two
types of them: `unique_ptr` and `shared_ptr` to capture exlusive ownership and
shared ownership, and makes it trivial to implement other tradeoffs such as
`boost::intrusive_ptr` and my `yasop`.

You may check the page on ownership [here](./concepts/resource-ownership.md)

With regards to the performance costs of smart pointers compared with 'naked'
pointers, we saw the following by looking at the assembler and assertions:

1. Every transfer ownership leaves a smart pointer without a resource that still
needs to call its destructor, which is deleting the null pointer.  That is,
basically one function call per ownership transfer
2. The AMD64 (for Intel and AMD processors) don't allow passing smart pointers
in registers to functions, nor returning them in registers.
3. Smart pointers, except for 2 above *do not have an extra indirection cost*

There are other costs or interface limitations specific to the smart pointer
itself:

* Instances of shared pointers are two-pointers in size, or double
the single object
* `shared_ptr`, `boost::intrusive_ptr` and `yasop` all
imply the overhead of a reference counter per instance handled.
* `shared_ptr` implies the use of atomic operations to increase/decrease the
reference counter
* `shared_ptr` has different performance with regards to operations involving
the reference count whether the instance was created with `make_shared` or `new`
* `boost::intrusive_ptr` is intrusive
* `yasop` can only handle instances created through `make_yasop`
* `yasop` only supports a subset of convertibility between derived and base
classes: when the base is at offset 0 in the derived.

#### CRTP

The curiously recurring template pattern:
We saw how in `boost::intrusive_ptr` and `yasop` there is the use of inheritance to 'inject' functionality into the derived class

