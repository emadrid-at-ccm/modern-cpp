#### Class #4

### Move semantics:

1. What is moving? Changing ownership
2. Ridiculousness of moving by copying/destroying resources
1. The syntax of `&&` for r-value references must not be confused with
`&&` in templates, which means forwarding or universal references.  Yet another
mistake.
2. "Moved from" state ambiguity
	1. Assignable
3. Penalty of unnecessary destructor
4. My hope that eventually this gets changed to no-state "pre-destroyed"

### The rule of zero, rule of three, rule of 5
[at cppreference.com]
(http://en.cppreference.com/w/cpp/language/rule_of_three)

### What is the cost of exception support?

1. Stack frames
2. Determining what to destroy
3. Destruction of frames is not part of the cost of exception support, but of
exceptions themselves

The old technique: thunking.  The new: "zero-cost": Doing searches in
collections of code address ranges.  [Interesting blog link]
(https://mortoray.com/2013/09/12/the-true-cost-of-zero-cost-exceptions/)

### Why not error codes

1. Intrusive
2. Error prone, manual
3. How do you propagate called errors to callers?
4. Anti-performance:
	1. Needs a test, even though the test will consistently go to the no-error
	branch, and thus the processor will successfully predict, it will still tax
	the branch prediction resources
	2. Makes the object code larger:
		1. zero-cost exceptions also make the code larger, but not the actually
		executed code, make the object code larger in an altogether different
		section
5. Make it harder to associate information about the context of the error:
	1. Global variables: thread unsafe, non-reentrant mechanisms `errno`
	2. Also requires the mapping from the code to the information
6. It does not enjoy the natural structuring of exceptions:
	* The standard library std::exception is a hierarchy!

### Dave Abraham's exception safety guarantees

1. No-throw
2. Strong exception safety: no side effects of failed operations
3. Basic exception safety: consistency and no resource leaks
4. No guarantee

How to achieve the strong exception safety?:  Transactional code:  Committ
only when you know no error will happen.

### Smart Pointers

* Very necessary
* `auto_ptr` was a failure
* boost `scoped_ptr`, `shared_ptr`, `intrusive_ptr`
* C++11:
	* unique_ptr
		* Requires *move semantics*
		* Cost?
	* shared_ptr
		* The common implementation is two pointers worth of space
	* There is no intrusive_ptr
		* intrusive_ptr has the issue of being intrusive!
			* So what?
	* yasop

War stories of implementing yasop:

* Straighforward and complex at the same time, teasing profound concepts
* For the first time ever, I had to make it so the compiler knew *less*
supporting dynamic casting
* Is `make_yasop` exception safe? -- if you don't believe me, how to fix it?
* My failure to achieve full pointer-to-base compatibility with pointer-to-
derived, at the same time, the merits of my partial success.
