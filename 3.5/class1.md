## C++ 3.5

#### This course is the third and a half iteration.

1. First iteration: pure C++

1. Second iteration: more practical C++

1. Second and a half iteration: more fundamentals

1. Third and a half: Mathematical and functional programming C++

#### Why C++?

*Mathematics is organized reasoning* by Richard P. Feynman, Nobel prize winner
in Physics, and a celebrity.

My doctrine of software engineering is that we deal with the complexity of
making billions upon billions of software elements work together.  The elements
in Software engineering are pure abstractions, and discrete.  Therefore,
programming is organizing software elements.

What are well organized software components?

What is accomplished by well organized software components?

"Premature optimization is the root of all evil", by Donald E. Knuth

The techniques to be studied do **not** have high performance as their direct
aim.  The aim is to empower the participant to organize software elements in
ways more optimal than practically feasible with other languages at the same
order of magnitude of performance.  This is an absolute, not relative aim.  That
is, its objective is not to allow participants to express things in C++, but
to express things in ways that are so nearly optimal that only C++ is a good
choice.  In particular, to achieve more reliable software spending less
effort.  Performance is an indirect consequence: If the techniques
do not have a performance cost by themselves, but allow better organized
software, then they allow improving the performance tradeoffs of implementations
and by informing the compiler more about what the program does which gives it the
opportunity to accomplish more micro optimizations.

I am a software performance expert, we can go full tilt in this topic if the
audience desires it, which allows learning advanced computing in general.

The three most important concepts are:

1. Coupling
2. Cohesion
3. Orthogonality

We will recurrently improve our understanding of them along the whole course.

C++ is among the top three languages I know to organize software components, the
other two are Lisp and dialects and Haskell.  Javascript is not far.  Perl is
extremely powerful but burdened by *accidental complexity*

* Lisp: Zero-syntax, then arbitrarily high levels of abstraction can be
accomplished by lisp code that generates lisp code that ... does something.
Macros.

* Haskell: Excellent representative of the "Functional Programming" paradigm.
*functional programming* (by the way, Lisp, Perl, Javascript are functional, not
C++) is characterized by functions being data elements.  Programs routinely
compose functions and treat them as any other value.  Attendant are concepts
like lexical closures: Just like in Object Orientation data structures have
their member functions; in Functional languages functions have their data
("*closures*")

None of Lisp, ... have zero-performance cost abstractions.  Increasing levels
of abstraction increase the performance penalty.  C++ is very singular in
allowing abstraction mechanisms that far from having a performance cost actually
have indirect performance benefits.  In a deeper sense, in my opinion Lisp,
Haskell are not true to the underlying computers, that's why their abstraction
layers have increasing costs; in C++, the abstraction layers are just *models*,
different ways to view the underlying computer, truly and simply more abstract.
There is little in terms of the capabilities of the computer that are not
accessible from C++, such as computed jump tables, but in my opinion it is so
feasible to make the features available that I have tought of branching clang
to implement them.

For an example of abstraction mechanisms, C++ relies on templates (a feature
that acts at compilation time) when other languages rely on runtime support;
C++ gives choices to programmers, other languages restrict choices; practically
all other languages rely on intrusive mechanisms while C++ is rather peculiar
in the natural way in which non-intrusive mechanisms allow the expression of
very powerful abstractions.

C++ is the best representative of "Generic Programming".  Roughly speaking,
making the types parameters of the code, or that the code does not concern
itself with irrelevant details of the types it deals with.  We will later
define what a data type is.  Generic Programming is to a degree
*metaprogramming*, making programs that reasons about themselves.
In any case, C++ code informs much more the compiler about what the program
does, then it helps detecting errors and optimizing.

### Epistemological theory of software engineering

One practical concern is to being able to express relationships between
software elements, if the relationships are not encoded then programmers can not
take advantage of them.  That is why in my software engineering doctrine I give
great importance to expressivity.  As elements of expressivity are introduced,
their benefits (or the cost of not having them) will be covered.

Complexity relates exponentially to the number of constituent software elements.
This relationship comes because integrating a new software element frequently
requires work linear with respect to the quantity of elements already present;
that is, in badly developed software, adding a new thing requires being careful
about the interactions between the new thing and all the preexisting ones.
If a function derivative is proportional to the function value, it is exponential.

I know of only one antidote to deal with the exponential relationship between
complexity and quantity of software elements:  Knowledge.  Knowledge is
exponentially powerful.  The reason why I give expressivity so much importance
is because knowledge can only be encoded with expressivity.  Applying my
epistemological theory of software engineering is my fundamental inspiration
for this course.

If you don't have knowledge to encode in your code, expressivity is a burden,
it leads to confusion. Fortunately, this is not our problem.  What we will be
doing, ultimately, is to learn how to express our knowledge into programming
code beyond what we've ever done.  Please be more demanding of yourselves in
terms of being able to express into programming code your knowledge.  You can
start by identifying the times you think you are "repeating yourself", when
you suspect you are programming again something you have already programmed.

Reputedly, J.R.R. Tolkien wrote the lore of what later became the
Silmarillion and probably all of the Lord of the Rings because for what he
really wanted to do, his elfish languages; otherwise they would have been an
empty exercise: a human language can not be any good if it does not have
lore, mythology, written in it.  The features in C++ that support programming
techniques are futile unless there are applications for them, I can introduce
the features, but they are solutions in search of problems, you supply the
problem.

I also become better every time I teach this subject because the
exposition of the techniques of how to encode knowledge in programming code
and submitting them to real life application in the course itself improves my
own understanding, competency.  I don't mind being challenged fairly.

I hate Python because it is an extremely poor language.  Being a poor language
makes it popular, because it does not require knowledge to use.  The drawback
is that it does not allow to express knowledge if you have it.  Popularity
causes a networking effect which expresses in the availability of exponential
quantities of effort.  Thus, a poor language like Python at the same time
suffers and benefits from the poverty of its expressivity.  I hate Java
vehemently, I consider it an intellectual vice because its asphixiating poverty
forces you to abuse Object Orientation to deal with challenges in the
territories where O. Orientation fails miserably, thus its programmer develops
increasingly bizarre modelling of software elements that become an intellectual
vicious cycle.  This opinion is informed by the fact that I used to be a
professional Java programmer.  As a matter of fact, my most successful
software engineering effort thus far has been leveraging a Java specific feature,
its "reflection" API.

#### Inherent versus Accidental complexities

I adopted the terms from former coworker Rohan Berdarkar: Accidental complexity
is the complexity as a result of mismodelling, things that are complicated
because of poor understanding.  C++ abounds in accidental complexity, not because
the poor understanding is the fault of its designers, but because its pioneering
history.  C++ acquired features to express promising programming techniques, but
the programming techniques were necessarily not well understood.  The cultural
choice made in the C++ community is to live with the mistakes, to not invalidate
code that relies on previous iterations of the language where things were not as
well understood.

For example, as Stephen Dewhurst put it, metaprogramming was *discovered*, not
*designed* into C++.  Because C++ pioneered support for Generic Programming
techniques, it acquired features powerful enough for crude metaprogramming,
and those techniques started to be used as in the technique of expression
templates that outperform even the best handcrafted code for linear algegra,
and then the community realized there were actual practical benefits to be
reaped from metaprogramming, despite its huge accidental complexity.
Once it was better understood, revisions such as C++ 11 made it two or
three orders of magnitude easier.

Dealing with C++'s accidental complexity is a different experience as in other
languages:  It helps you understand where the programming technique or the C++
feature fell short or manifests poor understanding; frequently a superior way
has also been developed, and thus the net result is a powerful learning
experience allowed by seeing how real life understanding of things improved over
time.

There are  we will see how the Standard Template Library is what I classify
as a major accomplishment of our civilization; despite how vastly superior it is
today to anything comparable developed in any other language in the nearly 20
years since it arose; currently C++ is on the verge of providing a significant
update that reflects a superior understanding of its merits, such as the concepts
of ranges.

Thus, learning the ugly and quirky parts of C++ makes you in absolute terms a
better programmer.

## C++ bad reputation

C++ is hard to understand, easy to misunderstand.  It is a language for
practitioners, hostile to theoriticians, and it does not have sponsorship of
any kind except us its programmers.

Most currently taught C++ is horrible bad.  The worst mistakes are emphasized as
the mainstream way to program in it, and the students or even the teachers don't
know any better.

Another related problem is that people who is completely ignorant of what C++
truly is sincerely think and have a public reputation of knowing what it is, so
their comments perpetuate an extreme misunderstanding.

The course has as its objective to empower the participants to see for
themselves the true power of C++, it succeeded in its previous two iterations,
thanks to previous success, I will raise the level of trust required to expose
this even more differently.

Mathematics is how our civilization organizes thought, this time I will be less
tolerant of proven bad approaches and will emphasize more the conceptual
dimension behind the good ideas.  There is an underlying conceptual continuum of
development behind C++ good ideas, that's where the emphasis will be made.  I
intend to use templates unapologetically, every time where it makes more sense
than not, as well as other techniques; you'll see how the correct application
of techniques have further consequences, including programming in a different
style.  I've seen that people are scared of embracing generic programming
because it is a very different way of programming; I have come to realize
the skills about doing generic programming relate to capturing the more
essential characteristics of the desired software elements, thus, generic
programming requires deeper reasoning about the thing being programmed, and
the result is better software.  **This course is very different** to the normal
way C++ is taught.  While the course is different, the conclusions are not
controversial, the recommendations are the same as those of the community and
have well agreed upon rationales.  I hope to be able to rely even more on
good internet presentations in this class than I did in the previous ones.
