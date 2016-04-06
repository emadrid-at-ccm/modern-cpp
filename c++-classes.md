#### Course outline:

C++ is very different to other programming languages.  It supports the styles of programming of many other languages, such as Java or C, so, most people learn or teach C++ as they would teach a more familiar language such as Java, and thus miss the way in which C++ things can be programmed in a superior way using its unique capabilities.  This is the first important mistake I want to avoid.  I will teach C++ by itself, in a very different way you would teach any other language.

To further complicate things, teaching the features is almost meaningless in the absence of well defined use cases; C++ is a language of practicioners, large swaths of modern programming techniques were *discovered* as opposed to *designed* into the language, such as the entirety of metaprogramming.  Thus, the use cases may need to be taught ahead of the language features, which implies learning C++ requires first becoming a better programmer, which is something I strongly believe is true.

To complicate things even more, there are features that are completely obsolete, such as mutable aggregate members and naked pointers, but since they are features as old as the language they have unbeatable syntactic convenience and popularity, which are both things difficult to fight against.

With the trust you are depositing in me to do things differently I will give you back guidance to learn efficiently, not just C++ but programming in general.

1. The most powerful feature of C++ is that resources can be handled automatically as the lifetime of objects.  Then, the first few sessions are to talk exhaustively about the lifetime of objects
	1. Handles
	2. Local variables in the presence of exceptions
	3. Error management digression
	4. The lifetime of components of partially constructed objects
	5. Dynamic objects
		1. Ownership
		2. Smart pointers

2. C++'s type system is one of the most powerful of any language, it can be used for:
	1. Detecting errors at compilation time
		1. CRTP
	2. Programming by pattern-matching
		1. Duck typing a la Python
		2. Overloads
		3. SFINAE (note: this is an advanced topic of templates, but not conceptually)
	3. STL
		1. Containers, algorithms, concepts
		2. Ranges
		3. Generators (infinite generators)
	4. Introduction to generic programming
	5. Introduction to meta programming

3. Functional programming
	1. Flaws and limitations of imperative programming and object orientation
	2. Template programming is functional programming
	3. Passing by value
	4. Preventing mutability
	5. Higher order functions
	6. Constexprs

4. Hourglasses

There are concepts and techniques that are "transversal" to most of the course, such as inheritance, inherent versus accidental complexity.

These topics can be taught in a number of ways, participants are encouraged to provide feedback so that I can make choices to get to the things most interesting to the participants the quicker way and to make more emphasis in them.
