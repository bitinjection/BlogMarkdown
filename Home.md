Title: Multiple Dispatch for the Masses
Author: Douglas Thompson
Date: 03-18-2015
Slug: multiple-dispatch

Popular modern day languages have progressed from predominantly procedural/imperative to single dispatch object oriented.  Most mainstream languages also incorporate functional and generic programming to a limited extent as well. We will worry only about the most common subset of features for this article.

The most common methods of dealing with multiple dispatch using these features will be shown.  These have been discovered through my own research and experience. Read through this with a critical mind, and be sure to diversify your sources on this topic.  The criticism sections following each approach will be the author's opinion, not actual facts.

This entry will utilize two languages with which most programmers will have at least a passing familiarity: C and Java.

Each of these techniques are fundamentally different.  While in many cases they are interchangable, there are instances where they are not.  For example, table based solutions allow the branching logic to be modified at runtime, while switch and object based solutions do not **inherently** support this.

I have included source code to demonstrate each of these approaches.  Code is provided in C or Java, depending on the features needed for each technique.  The article is written without referencing the code directly to allow both the code and article to exist independently.

# What is Multiple Dispatch?

Code is often written against abstractions.  Abstractions are helpful in the development process, but they all must eventually be resolved to a concrete type in order to do actual work.

## A Single Dispatch Example

As an example from an object oriented language, an `Animal` abstraction may provide a `speak` method.  A programmer may write
```java
animal.speak();
```
If an Animal is a purely abstract object, then its `speak` method will have no actual code.  At runtime, the program will dynamically dispatch to object's underlying type and call that object's `speak` method.  If the abstract `animal` object points to an instance of `Dog` class, then the above call may print `woof` to the screen.  The above call might effectively be translated to
```java
dog.speak();  // prints "woof"
```

# A Multiple Dispatch Example
At times it my be necessary to dispatch based on multiple types.

A programmer writing a game of rock-paper-scissors might use an abstraction of `Attacker` to represent a potential move.  When the actual game is played the type of both attackers will need to be known.  What we **want** to write is something along the lines of:
```java
(attacker1 and attacker2).determineWinner();
```
`attacker1` might be resolved to `Rock` and `attacker2` might be resolved to `Scissors`.  In this case, the code relating to rock beating scissors would be called.

# Multiple Dispatch in C
There are two main ways to go about this in C.  Both of these involve defining types which explicitly describe themselves.  An enum may be defined as
```c
enum { ROCK, PAPER, SCISSORS} attacker;
```
The types for each attack could be defined by
```c
struct { attacker TYPE = ROCK; }
struct { attacker TYPE = PAPER; }
struct { attacker TYPE = SCISSORS; }
```
## Big Switch Statement

The typically cringeworthy solution is to have a Big Switch Statement to handle every possible combination.  In psuedocode, for sanity related reasons
```c
switch attacker1.TYPE:
  case ROCK:
    switch attacker2.TYPE:
      case ROCK: tie
      case PAPER: lose
      case SCISSORS: win
  case PAPER:
    switch attacker2.TYPE:
      case ROCK: win
      etc...
  etc...
```
This switch statement ends up being huge with only three types, and the complexity grows exponentially as types are added.

Beyond factoring each switch and/or case statement into its own function, the switch statement itself may be replaced with a jump table.

### Criticisms 
Good

* Not **too** difficult to manually trace through

Bad

* Adding/removing types requires a lot of work which won't be verified by a compiler
* Have to add a type field to all types and keep up with these
* Switch statements tend to poliferate

The first bullet under the negative criticisms is particularly important.  Adding or removing types involves modifying a lot of code spread out all over your program, and the compiler will not let you know if you have left anything out in many cases.

## Big Jump Table
The logic of a switch statement or chain of if/else statements based on explicitly defined types may be replaced by a jump table.  Imagine a matrix of possible outcomes for the game itself

- | Rock | Paper | Scissors |
- | - | - |
**Rock** | Tie | Lose | Win |
**Paper** | Win | Tie | Lose |
**Scissors** | Lose | Win | Tie |

We can translate this into code by creating functions for every possible combination of rock-paper-scissors, then populating a table based on the intersection of the types.  Dispatching to the correct function can then be done by looking up the function pointer in the table
```c
BattleFunction generate_outcome = gameMatrix[attacker1.TYPE, attacker2.TYPE];
generate_outcome();
```
Maintaining the `TYPE` field in all of our objects is still a bit of a pain.  If we were to add or remove any types, we would have to edit our enum and update our matrix.

### Criticisms
Good

* Tables can be constructed outside of static code.  This consolidation makes modification easier and will usually not even require modification of the code itself.
* Leads to simpler code.  Functions which would typically contain large switch statements simply index into a table.

Bad

* Much more difficult to debug/trace.
* Still requires types to label themselves.

Again, the first bullet above is worth reading over again.

# Introspection
Introspection may be used to help alleviate some of the issues with the C-based approaches to this problem when applicable.

Instead of manually tracking each type, the underlying type of each abstraction is determined via a language's introspection features, and then the relevant code is dispatched to based on the findings.

> As a side note, this approach is often times discovered independently and immediatelly touted as a panacea for the ugly switch/table based methods previously described, and the also ugly object-based double indirection used in object oriented languages (described in a moment).  It is my opinion that it is not.  While it does simplify some aspects of the table-based approach, it still carries many of the drawbacks associated with that method, and it introduces many of its own drawbacks in the process.  These will be detailed in the general compare/contrast of each method toward the end.

## Criticisms
Good

* Removes the need for types to keep track of themselves.

Bad

* Makes debugging/tracing even more painful.
* While explicit tracking of types is removed, the developer must still be mentally aware of what these types are when updating tables.

# Object Oriented Approach
In object oriented languages, polymorphism is the go-to mechanism for replacing branches based on type.  To implement double dispatch, one can first write something along the lines of
```java
attacker1.handle(attacker2);
```
This dispatches to the appropriate `handle` method based on attacker1's type.  If `attacker1` is a `rock`, for example, you could mentally translate this to
```java
rock.handle(attacker2);
```
Once we're in the Rock's `handle` method, we know we are a rock.  Now all that's left to do is figure out what attacker2 is.  We can do this in similar fasion inside of the rock.handle method
```java
attacker2.handleRock(this);
```
We know that we're a `rock`, so `this` is really `rock`.  Again, if `attacker2` is **actually** `paper`, the call may be mentally translated to
```java
paper.handleRock(rock)
```
And in our Paper class, the handleRock method can contain the code which handles the rock versus paper interaction.  Function overloading is often used during this second dispatch, but it is not necessary for the language to support overloading for this technique to work.

## Criticisms
Good

* Returns many of the potential errors to compile time
* Avoids the poliferation inherent in switch-based solutions

Bad

* Can seem convoluted to the uninitiated
* It is an object oriented solution which violates many object oriented ideas
* Costs associated with adding/removing types vary based on which set of types you are modifying

I will spend the second half of this article examining this object oriented approach, as I find it to be heavily criticized in ways which I think are unfair.  If the above bullets are not clear, feel free to read on.

# Common Object Oriented-based Complaints

Hopefully it is now clear that there are no clear winners when it comes to emulating multiple dispatch.  Many would have you believe, however, that the object oriented solution is a clear loser.  While it is by no means perfect, I feel many complaints are exaggerated or meaningless.

Though it will be discussed why later, many of the complains about the Visitor pattern will be addressed here as well.

## It Breaks Encapsulation

This argument is absolutely true.

Object oriented programmer is centered around the idea that a set of data and the functions which operate on it should be packaged together.  The public interface of the object protects the data from change which might put the object into an invalid state.

When we use multiple dispatch, though, we call a function which is supposed to interact with more than one type.  In other words, the function itself does not belong to any one object--rather it belongs to **both objects**.  Since the language does not inherently support multiple dispatch, there is usually no facility to create a function which is somehow owned by multiple objects.

I really have no counter argument here, but it is important to note that none of the other methods are any better in this respect.  While the object oriented approach leverages the mechanics of objects, this does not mean that the author is attempting to shoe-horn the dispatch function into the object oriented paradigm.

## You Don't Need It

The idea here is that if you're attempting to implement multiple dispatch, there is going to be some other way which is simpler/better that you just aren't using.  This is a particularly common complain when discussing the Visitor pattern (discussed later).

The problem with this complaint is that it is just wrong.  Situations which require multiple dispatch **really do require multiple dispatch**.

Most of these complaints stem from improper the abuse of the Visitor pattern.  Be very wary when reading about these--just because a person has improperly picked a pattern, and then that person later on finds a better way of solving their problem, does not mean that the initial solution is A Bad Thing that should never be used.

## The Dispatch Functions Grow Combinatorially

This one is another complaint that's absolutely correct.  What drives me nuts is that it is a complaint!

Consider our rock-paper-scissors game.  Without going into the math behind it (though it is very simple), you can see from our matrix that there are three possible ways to attack, for a total of nine possible game-handling function calls.  If we were to add another way to attack, we would have **sixteen** functions to write.

In more complex games, such as any that you might find on the shelf in a store, there may be 20 or more potential entities to consider.  For 20 entities, that's up to **four hundred** collision handling functions.

Needless to say, I can see why people freak out about this exponential growth in the number of necessary functions.  Unfortunately, **that's life, deal with it**.  If you have a game with 20 entities flying around, you just have to deal with them colliding.  There's no real way around it.

Table-based methods have the advantage that not all cells need to be populated, but this just takes us back to the initial trade-off--an object-based design would require 400explicit function definitions, while a table based method would only require the ones that will actually ever be called (but you better hope you don't accidentally try to collide two objects that don't have a handler).

I am not claiming that either approach is superior to the other, but the number of situations you need to mentally handle will grow expontentially whether you like it or not.

## Open/Closed Concerns

This one is actually tricky, legitimate, and will force me to finally bridge the gap between the general case of multiple dispatch and the Visitor pattern.

The rock-paper-scissors example is a special case.  In this instance, we are dispatching to two different types, each from the same group of types.  In other words, a rock, paper, or scissors will always be up against a rock, paper, or scissors.  Adding or removing a possible attack is going to cause a lot of code to shift around.  Having to chance code in classes that deal with rock/paper/scissor attacks everytime a new attack is added or removed actually makes sense, and there probably are not many people that would complain about having to do this.

The types dispatched to do not always have to be from the same group, though.  A common example is that you have a group of heterogeneous types (with a common interface), and you want some way to render these.  In this case, we have two seperate groups of types which we want to dispatch to.  

The usual example for this is some sort of drawing program.  For example

Shapes
* Circle
* Square
* Line

Renderers
* Win32
* GTK
* ASCII

A `Renderer` would have methods which render each type of `Shape`, and each `Shape` would simply provide the secondary dispatch function (`renderer.visit(this)`).

The complaint is this: adding/removing a `Renderer` is insanely easy, but adding/removing a `Shape` is insanely difficult.  Adding/removing a `Renderer` is as simple as adding or removing the class from the project.  Adding/removing a `Shape` requires that **every Renderer object be modified**.

My guess is that this asymmetry is where a lot of the convoluted definitions and flawed understandings originate.  It is my opinion that if multiple dispatch is being used correctly, then the need to modify so many classes is ultimately justified.  In the classic Shape/Renderer example above, you would **want** to modify all of your renderers when a shape is added/removed.


