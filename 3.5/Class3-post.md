# Type dependent code reuse presentation

We watched [Mark Isaacson's presentation on type-dependent code reuse](https://vimeo.com/131194141)

And we had a digression on the subject of automatic deduction of template arguments.

From the point of view of course style, I prefer to introduce progressively the concepts on the spot.  We are *practitioners* of programming, and I think this is the way we learn best.  Objections to the lack of organization may be founded, please provide feedback to that regard.

IMO the presentation should drop the "type dependent" qualifier.  The techniques mentioned in the presentation:
 
- Overloads
- Templates
- Type dispatch
- Traits
- Inheritance
- CRTP
 
Are just tools in modern C++, they are all type-dependent because in modern C++ one plays with the types all the time.
 
### Overloads
 
In C++ the same function name can be used with different types.  The compiler will match the types given in the call to the *most specific match* among the definitions.

All of the definitions that match the call are called *the overload set* or the *set of viable overloads*.  From the overload set the compiler will select the *most specific* to then use it to translate the call itself.  Intuitively, one overload `o1` is more specific to `o2` if `o1` can be converted to `o2` but not the other way around.  For example, pointers are all implicitly convertible to `const void *`, given these two overloads:

`void doSomethingWithPointer(const void *) { calledWithVoidPointer(); }`
and
`void doSomethingWithPointer(const int *) { calledWithIntPointer(); }`

If `dp` is a pointer to double, and `ip` a pointer to integer, *not constants*, a call such as
`doSomethingWithPointer(dp)` does not have an exact match among the two overloads given, but a `double *` is not implicitly convertible to `const int *`, therefore, will have only one *viable overload*, and that will be the one with `const void *`, in the call the `dp` will be implicitly converted to `const void *`.  For `doSomethingWithPointer(ip)` the viable set has both overloads, but an `const int *` can be converted to `const void *` but not the other way around, therefore the overload with `const int *` is *more specific*.

I consider the rules for overload resolution as **clearly the most complicated topic in all of C++**.  The benefit from that complexity gives us:

1. A very powerful pattern matching mechanism
2. An extensibility mechanism (that is, to introduce new functionality to existing code without changing the existing code)
3. Syntactical convenience, such as the benefits of operator overloading
4. Generic programming: We can assign *semantics* to a name that will be adapted to each specific set of arguments, such as we don't need to know what are the types of `+`, we expect the right form of addition to happen.  Without overloads the only way to do any generic programming is through *type erasure*, that is, the equivalent of converting as in plain C to `const void *` or to `Object` in Java (I think for the specific use case of `max`, the type erasure would be to convert an object to the interface, I think `Comparable`, it does not matter, the principle is the same, type information is *lost* to the compiler).  This form of type erasure is inherently error prone.

Unfortunately, for all of its greatness, and inherent complexity, overload resolution rules in C++ abound with historical artifacts, language rules such as "Koenig's Lookup" (rules for when the function overload resolution looks into namespaces for matches, in honor of Andrew Koening); that after twenty years how to solve some annoyances continue to be open questions much debated.

### Templates
 
For the code:
 
```c++
template<typename T> T max(T a1, T a2) { return a1 < a2 ? a2 : a1; }
```
 
What is required of the type that may participate in a call to `max`?
Nothing.  The type is not restricted in any way.
 
However, once overload resolution has selected that template, when compiling the call more requirements arise:  both arguments must be of the same type (that is what the code says, whatever the template type argument, both arguments to the function call are of the same type).  Furthermore, the type is *passed by value*, then the type needs to be copiable.  Then, in the *implementation* of the function, the requirement of the values to be comparable through `<`.  Please note there is a big difference between the *selection of an overload* and once the overload has been selected.  We will see much later that there is a technique called "SFINAE", which stands for the undecipherable acronym "Substitution Failure Is Not An Error", which is a fundamental pattern matching mechanism: Failure to select an overload is no error.  Once an overload *is* selected, *any* error is a "hard error", the program fails to build.
 
### Type Dispatch
 
The technique of implementing a function by calling an implementation function with extra arguments that only convey type information to the compiler, typically, types represented by empty classes.  The extra arguments activate different patterns, type dispatch is one way to do pattern matching at compilation time.

### Traits

Properties of types.  For example, `std::is_trivially_copiable`, `std::is_base`, `std::is_trivially_constructible`.  When doing generic programming, one needs to "branch" on the properties of the types.

### Inheritance, CRTP

Deferred to classes later.
