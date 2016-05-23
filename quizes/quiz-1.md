#### Automatic conversions

Please mark the items that are not valid implicit conversions.  Also indicate why the proposed conversion is not implicit

1. int to double
2. double to int
3. pointer to any type to bool
4. bool to some pointer type
5. `int *` to `void *`
6. `void *` to `char *`
7. `const int *` to `void *`
8. `volatile int *` to `void *`
9. `int *` to `volatile const void *`
	1. By the way, `volatile const` pointers are syntactically valid.  Does it make sense to have a `volatile const` pointers?
10. `int *` to `const int *`
11. `int *` to `const void *`
12. `int **` to `const int **`
13. `int **` to `int *const *`
14. `int **` to `int **const`.  Please observe these three examples have the `const` in different positions, do you know what each position means?
15. `int *` to `char *`
16. `char *` to `int *`
17. `double (*)(int)` to `void *` (this is a pointer to a function that takes int and returns double)
18. `int (*)(int)` to `const void *`


For the following items, assume that `struct GrandDaughter` inherits directly and publicly from `struct Derived` which inherits directly and publicly from `struct Base`

12. `Derived *` to `Base *`
13. `Base *` to `Derived *`
14. `GrandDaughter *` to `const Base * const`.
15. `Derived **` to `Base **`

What is the meaning of "covariant with respect to inheritance"?
What is the meaning of "contravariant"?

16. `std::shared_ptr<Derived>` to `std::shared_ptr<Base>`
17. `std::shared_ptr<GrandDaughter>` to `std::shared_ptr<Base>`

The following code compiles, do you know why? what does it do?

	using callback_t = bool (*)(void *);

	template<typename T> callback_t make_callback() {
		return [](void *arg)->T {
			callback_t a = reinterpret_cast<callback_t>(arg);
			return a(nullptr);
		};
	}

	callback_t status_check = make_callback<bool>();

If there is a "free standing function" `bool default_status_check()`, what will this line do?

	status_check(default_status_check);

Assume `SomeType` defines a instance member function `bool compare(SomeType) const`

Should the following compile (is it a reasonable expectation that it compiles)?

	template<typename Iterator, typename Callable>
	void mysort(Iterator begin, Iterator end, Callable v);
	void fun(SomeType *start, SomeType *end, bool direct) {
		mysort(
		    start,
		    end,
		    [=](SomeType p1, SomeType p2)->bool {
		        return direct ? p1.compare(p2): p2.compare(p1);
		    }
		);
	}

Is there any objection to the code above you can think?

What about this, does it compile?:

	template<typename Value> void mysort2(const Value *, const Value *, bool (*compare)(Value, Value));

	void fun2(SomeType *start, SomeType *end) {
		mysort2(start, end, [](SomeType p1, SomeType p2)->bool { return p1.compare(p2); });
	}

Consider this code for the following questions:

	#include <string>

	void call_on_reference(int &);
	void call_on_const_reference(const int &);

	struct Info;

	struct Token1 { Info *info; };
	struct Token2: Token1 {};
	struct Token3 { Info *info; };

	struct A {
		A(Token2);
		A(Token3);

		Info *i;
	};

	struct B {
		B(A);
		explicit B(Token1);

		operator A() const;		
		operator std::string() const { return "B"; }

		Info *i;
	};

	struct C {
		operator B() const;
		Info *i;
	};

	void consumes_B(const B &);

	Token1 theToken1;	
	A theA{Token2{}};
	C theC;

	#include <iostream>

	template<typename T> consumption(const T &v)
	{ std::cout << std::string(v); }

	void consumption(const B &);


1. Will this call `call_on_reference(5)` succeed to compile?
2. What about `call_on_const_reference(5)`?
3. Implicit conversion of `Token2` to `Token1`
4. Implicit conversion from `A` to `B`
5. Implicit conversion from `Token` to `B`
6. Does this compile `consumes_B(theA);`?
7. `consumes_B(theToken1);`
8. `consumes_B(theC);`
6. Does this compile `consumption(B{theA});`?
7. `consumption(theToken1)`;
8. `consumption(theC)`;


