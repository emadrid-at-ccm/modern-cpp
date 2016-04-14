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

It is always possible to get the pointer associated with a reference, use the `&` operator.

All the things mentioned apply to r-value references too, but r-value references have the same
syntax as forwarding references (also called *universal* references).
