### Differences between pointers and references

References are pointers, the important differences are:

1. References must refer to a valid object, pointers can be null
2. References are already de-referenced syntactically:

        auto &reference = something; // 'reference' is a reference to 'something'
        auto somethingPtr = &something;
syntactically, using `reference` is exactly the same as if you'd use `(*somethingPtr)`
3. References can not be rebound, pointers can be changed to point to something else
4. `auto` changes a reference to a value!:

        something &this_function_returns_a_reference();// ...
        void some_function() {
            auto v = this_function_returns_a_reference(); // <- 'v' is NOT a reference but a copy!
            auto &r = this_function_returns_a_reference(); // <- 'r' is a reference
5. Non-modifiable references (`const` references) can bind to constants that don't even
have a location in memory, that is, this works: `const int &cir = 5`

Since passing by value implies copying the values, in traditional C++ we have adopted the idiom
of passing by const-reference as a micro-optimization instead.  As seen above, one can bind
const-references to constants but for a constant to be bound to a const-reference, you should
know the compiler must create the value in memory, and then pass not the value but the address;
the potential problem is that as time goes by, the *register file* of the processors become
ever larger, so, the processor can keep larger and larger sets of values in the registers ready
for operations, then *indirecting* through memory may prevent the compiler from using a value
it already has in the registers, and force it to reload it from memory all the time, which
at that point becomes a pessimization.

It is always possible to get the pointer associated with a reference, use the `&` operator.

All the things mentioned apply to r-value references too, but r-value references have the same
syntax as forwarding references (also called *universal* references).  In the following
code to give a sample for what forwarding references are, I will "ask" the compiler to give
me the types of things by triggering a compilation error, attempting to instantiate the
template `Trick` which is declared but not defined.  Please study all four compilation
errors, three of them are to see types, and the fourth is to show how modifiable
(non-`const`) references can not bind to compile time values.

	template<typename T> struct Trick;

	template<typename T> void just_a_type(T v) { Trick<T> error; }

	template<typename T> void forwarding_reference_example(T &&v) { Trick<decltype(v)> q; }

	void modifiable_int_reference(int &v);

	void unmodifiable_int_reference(const int &v);

	void fun() {
		int v = 5;
		int &intref = v;
		just_a_type(intref);
		forwarding_reference_example(v);
		forwarding_reference_example(5);
		modifiable_int_reference(v);
		modifiable_int_reference(intref);
		modifiable_int_reference(5);
		unmodifiable_int_reference(5);
	}


Link to compiler explorer [here](http://gcc.godbolt.org/#compilers:!((compiler:g530,options:'-std%3Dc%2B%2B11+-Wall+-O3',sourcez:C4UwtgDgNghqA8wCeEQDsZhAAgCoD5sBnYAJwFcBjYPUgS0oGsBuAKFdElgWVQyzyEAbgHs6AE2wArciQD6MObxAAKXNiEBKbAG9aDRvALYQpUiNLNsAX3adocEIhTpMOY6InYAZhYDuMKTidGgA5nKkIN6m6JQgciAAHpjQquoAZOlauvpM8OIglFDKKlqEAI5WtqyekmAiwd50MABGUPEhwBFRMWhxKp3YWZpsNWKS5Gj1jc1tHWhdkdGRfaqUImgk2IPDo7U%2BkyraOqzY2wsa2AC82ACsbGc7nUvXGg/Ssl2KJc9RI6c%2BfyBYJhbrLWLxJIpdqlf5nXykAJBELhJa9OIJZJcVS3OHYaZ0JqtdpyTpg9GqLTvAlEuakhbklb9X7ePE02Ykslopk4vGTdnE%2BaLHo8lS4ti2IAAA)),filterAsm:(commentOnly:!t,directives:!t,labels:!t),version:3)
