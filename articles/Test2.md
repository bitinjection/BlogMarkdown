Title: State Pattern
Author: Douglas Thompson
Date: 04-21-2015
Slug: state-pattern
Category: Patterns

Popular programming languages used have progressed from perdominately procedural/imperative to single dispatch object oriented.  Most mainstream languages also incorporate functional and generic programming to a limited extent as well. We will worry only about the most common subset of features found in C++, C#, and Java for this article.

The most common methods of implementing multiple dispatch using these languages will be shown.  Keep in mind that these have been discovered through my own research and experience, so read through this with a critical eye.

This entry will utilize two languages with which most programmers will have at least a passing familiarity: C and Java.  C is used to demonstrate how the problem may be approached in a language without objects, and Java is used to demonstrate how the problem may be approached using object oriented practices.  Each of these techniques are fundamentally different.  While in many cases they are interchangable, there are instances where they are not.

I have included source code to demonstrate each of these approaches.  Code is provided in C or Java, depending on the features needed for each technique.  The article is written without referencing the code directly to allow both the code and article to exist independently.

# What is Multiple Dispatch?

Abstractions are helpful in the development process, but they all must eventually be resolved to a concrete type in order to do actual work.

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
It may be necessary to dispatch based on multiple types in many situations.

A programmer writing a game of rock-paper-scissors might use an abstraction of an `Attack` to represent a potential play.  When the actual game is played, the type of both attacks will need to be known.  What we **want** to write is something along the lines of:
```java
(attack1 and attack2).determineWinner();
```
`attack1` might be resolved to `Rock` and `attack2` might be resolved to `Scissors`.  In this case, the code relating to rock beating scissors would be called.

# Multiple Dispatch in C
There are two main ways to go about this in C.  Both of these involve defining types which explicitly describe themselves.  An enum may be defined as
```c
enum { ROCK, PAPER, SCISSORS} attack;
```
The types for each attack could be defined by
```c
struct { attack TYPE = ROCK; }
struct { attack TYPE = PAPER; }
struct { attack TYPE = SCISSORS; }
```
## Big Switch Statement

One solution is to have a Big Switch Statement to handle every possible combination of attacks.  In psuedocode:

```c
switch attack1.TYPE:
  case ROCK:
    switch attack2.TYPE:
      case ROCK: tie
      case PAPER: lose
      case SCISSORS: win
  case PAPER:
    switch attack2.TYPE:
      case ROCK: win
      etc...
  etc...
```
This switch statement ends up being huge with only three types, and the complexity grows exponentially as types are added.

The switch statement itself may be replaced with a jump table if preferred.

### Criticisms 
Good

* Not **too** difficult to manually trace through

Bad

* Adding/removing types requires a lot of work which will not be verified by a compiler
* Have to add a type field to all types and keep track of all types
* Switch statements tend to poliferate and manifest as duplicated code throughout the project

## Big Jump Table

Imagine a matrix of possible outcomes for the game itself:

- | Rock | Paper | Scissors |
- | - | - |
**Rock** | Tie | Lose | Win |
**Paper** | Win | Tie | Lose |
**Scissors** | Lose | Win | Tie |

We can translate this into code by creating functions for every possible combination of rock-paper-scissors. We then populate a table based on the intersection of the types.  Dispatching to the correct function can then be done by looking up the function pointer in the table
```c
BattleFunction generate_outcome = gameMatrix[attack1.TYPE, attack2.TYPE];
generate_outcome();
```
Maintaining the `TYPE` field in all of our objects is still a bit of a pain.  If we were to add or remove any types, we would have to edit our enum and update our matrix.

### Criticisms
Good

* Tables can be constructed outside of static code.  Modification of the table does not usually require modification of the code which uses the table (in contrast to the switch statement approach).

Bad

* Much more difficult to debug/trace.
* Still requires types to label themselves.

# Introspection
Introspection may be used to help alleviate some of the issues with the C-based approaches to this problem.

Instead of manually tracking each type, the underlying type of each abstraction is determined via a language's introspection features, and then the relevant code is dispatched to based on the findings.

> As a side note, this approach is often times discovered independently and immediatelly touted as a panacea for the ugly switch/table based methods previously described, and the also ugly object-based double indirection used in object oriented languages (described in a moment).  It is my opinion that it is not.  While it does simplify some aspects of the table-based approach, it still carries many of the drawbacks associated with that method, and it introduces many of its own drawbacks in the process.

## Criticisms
Good

* Removes the need for types to keep track of themselves.

Bad

* Makes debugging/tracing even more painful.
* While explicit tracking of types is removed, the developer must still be mentally aware of what these types are when updating tables.

# Object Oriented Approach
In object oriented languages, polymorphism is the go-to mechanism for replacing branches based on type.  To implement double dispatch, one can first write something along the lines of
```java
attack1.handle(attack2);
```
This dispatches to the appropriate `handle` method based on attack1's type.  If `attack1` is a `rock`, for example, you could mentally translate this to
```java
rock.handle(attack2);
```
Once we're in the Rock's `handle` method, we know we are a rock.  To determine what `attack2` is, we can write
```java
attack2.handleRock(this);
```
We know that we're a `rock`, so `this` is really `rock`.  If `attack2`'s underlying type is `paper`, the call may be mentally translated to
```java
paper.handleRock(rock)
```
And in our Paper class, the handleRock method can contain the code which handles the rock versus paper interaction.  

> A common misunderstanding is that this technique only works in a language which supports function overloading.  As shown above, it does not.  Function overloading is often used during this second dispatch, but it is not necessary for the language to support overloading for this technique to work.

## Criticisms
Good

* Returns many of the potential errors to compile time
* Avoids the poliferation inherent in switch-based solutions

Bad

* May be difficult to understand if unfamiliar with the design pattern
* It is an object oriented solution which violates many object oriented ideas

> I will spend the second half of this article examining this object oriented approach, as I find it to be heavily criticized in ways which I think are unfair.  If the above bullets are not clear, feel free to read on.

# Common Object Oriented-based Complaints

Hopefully it is obvious that there are no clear winners when it comes to emulating multiple dispatch.  Many would have you believe, however, that the object oriented solution is a clear loser.  While it is by no means perfect, I feel many complaints are exaggerated or meaningless.

Though it will be discussed why later, many of the complains about the Visitor pattern will be addressed here as well.

## It Breaks Encapsulation

Object oriented programming is centered around the idea that a set of data and the functions which operate on it should be packaged together.  The public interface of the object protects the data from change which might put the object into an invalid state.

When we use multiple dispatch, though, we call a function which is supposed to interact with more than one type.  In other words, the function itself does not belong to any one object--rather it belongs to **both objects**.  Since the language does not inherently support multiple dispatch, there is usually no facility to create a function which is somehow owned by multiple objects.

I really have no counter argument here, but it is important to note that none of the other methods are any better in this respect.  If multiple dispatch is truly necessary, you will not find an encapsulatable approach.

## You Don't Need It

The claim here is that if you're attempting to implement multiple dispatch, there is going to be some other way which is simpler/better that you just aren't using.  This is a particularly common complain when discussing the Visitor pattern (discussed later).

The problem with this complaint is that **it is just wrong**.  Situations which require multiple dispatch **really do require multiple dispatch**.

Most of these complaints stem from improper use of the Visitor pattern.  Be very wary when reading about these--just because a person has improperly picked a pattern, and then that person later on finds a better way of solving their problem, does not mean that the initial solution is A Bad Thing that should never be used.

## The Dispatch Functions Grow Combinatorially

Consider our rock-paper-scissors game.  Without going into the math behind it (though it is basic high school combinatorics), you can see from our matrix that there are three possible ways to attack, for a total of nine possible game-handling function calls.  If we were to add another way to attack, we would have **sixteen** functions to write.  
In more complex games, such as any that you might find on the shelf in a store, there may be 20 or more potential entities to consider.  For 20 entities, that's up to **four hundred** collision handling functions.

Unfortunately, **that's life, deal with it**.  If you have a game with 20 entities flying around, you have to deal with them colliding.  There's no way around it.

Table-based methods have the advantage that not all cells need to be populated, but this just takes us back to the initial trade-off--an object-based design would require 400explicit function definitions, while a table based method would only require the ones that will actually ever be called (but you better hope you don't accidentally try to collide two objects that don't have a handler).

I am not claiming that either approach is superior to the other, but the number of situations you need to mentally handle will grow expontentially whether you like it or not.

## Open/Closed Concerns

The rock-paper-scissors example is a special case.  In this instance, we are dispatching to two different types, each from the same group of types.  In other words, a rock, paper, or scissors will always be up against a rock, paper, or scissors.  Adding or removing a possible attack is going to cause a lot of code to shift around.  Having to chance code in classes that deal with rock/paper/scissor attacks everytime a new attack is added or removed actually makes sense, and there probably are not many people that would complain about having to do this.

The types dispatched to do not always have to be from the same group, though.  A common example is that you have a group of heterogeneous `Shape` types, and you want to render these using a variety of methods.  In this case, we have two seperate groups of types which we want to dispatch to.  

For example:

Shapes
* Circle
* Square
* Line

Renderers
* Win32
* GTK
* ASCII

A `Renderer` would have methods which render each type of `Shape`, and each `Shape` would simply provide the secondary dispatch function (`renderer.visit(this)`).

The complaint is this: adding/removing a `Renderer` is easy, but adding/removing a `Shape` is difficult.  Adding/removing a `Renderer` is as simple as adding or removing the class from the project.  Adding/removing a `Shape` requires that **every Renderer object be modified**.

My guess is that this asymmetry is where a lot of the convoluted definitions and flawed understandings originate.  It is my opinion that if multiple dispatch is being used correctly, then the need to modify so many classes is ultimately justified.  In the classic Shape/Renderer example above, you would **want** to modify all of your renderers when a shape is added/removed.  Even if there were hundreds of `Renderer`s, you would want to make sure that each of them could render all of the necessary types.

# Alternatives

Not many heavily used languages curently have official, explicit support for multiple dispatch, but many have ways to facilitate the process beyond what has been described here:

* C# has the `dynamic` keyword
* Python has libraries leveraging the language's decorator mechanism
* C++ libraries enable multiple dispatch via template metaprogramming

# Conclusion

Multiple dispatch, in my opinion, is massively misunderstood, incorrectly used, and possibly underutilized.  This is no surprise, however, as the mechanisms for supporting such a concept in mainstream languages are heavily lacking.  Still, recognizing the situations in which multiple dispatch may be needed, and understanding the possible approaches to implementing it in your environment, can prevent the production of otherwise unweildly, hackneyed code which might attempt to be something that it is not.
