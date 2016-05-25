#### Conversions

1. There is no implicit conversion from base class to derived class.  Assume
`Derived` is a class that publicly inherits from `Base`, and `base` is a
reference to `Base`:
	1. `static_cast<Derived &>(base)` works and is safe
	2. `reinterpret_cast<Derived &>(base)` works and is safe
	3. `const_cast<const Derived &>(base)`
	4. `dynamic_cast<const Derived &>(base)`
	5. What does the c-style cast would do? `auto &v = (Derived &)base;`
	6. Which of the explicit conversions imply a performance penalty?
	7. When is it safe to assume `Derived` begins with the instance data members
	of `Base`?
	8. Is it possible to programmatically find out whether a base class is the
	beginning of the memory layout of a derived type?

2. Show the different choices the programmers have to convert a value of type
"A" into a value of type "B".
	1. Which lead to implicit conversions?

#### Overloads, first part

1. Provide an example of overload ambiguity
2. `void fun(int); void fun(double);`, which overload will be picked by the
call `fun(5)`?

From the following code:

	struct token {};
	struct subtoken: token {};

	int priority(...) { return 0; }
	int priority(const void *) { return 1; }
	int priority(void *) { return 2; }
	int priority(const int *) { return 4; }
	int priority(int *) { return 5; }
	int priority(token *) { return 6; }

1. Make examples of calls to `priority` that give each of the values.
2. Provide an example of a pointer type such that a value `v` of that type has
`priority(v)` be neither 1 nor 2
3. Verify with compiler or any tool that `void **` hits overload for `void *`
4. What is the value of `priority((subtoken *)0)`?

We have not studied the topic of template patterns, however, please reason
about the following questions:
Given the extra code `template<typename T> int priority(T *) { return 7; }`

1. What is the result of `priority((subtoken *)0)`
2. What is the result of your answer for question #2 (pointer that hits neither
of `void *` nor `const void *` overloads)?
