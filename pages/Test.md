Title: Multiple Dispatch for the Masses
Author: Douglas Thompson
Date: 03-18-2015

Popular modern day languages used by most programmers have progressed from perdominately procedural/imperative to single dispatch object oriented.  Most mainstream languages also incorporate functional and generic programming to a limited extent as well. We will worry only about the most common subset of features for this article.

The most common methods of dealing with multiple dispatch using these languages and features will be shown.  Keep in mind that these have been discovered through my own research and experience, so read through this with a critical mind, and be sure to diversify your sources on this topic.

This entry will utilize two languages with which most programmers will have at least a passing familiarity: C and Java.

# What is Multiple Dispatch?

Code is often written against abstractions.  These abstractions are helpful in the development process, but they all must eventually be resolved to a concrete type in order to do actual work.

## A Single Dispatch Example

As a quick example from an object oriented language, an `Animal` abstraction may provide a `speak` method.  A programmer may write:
```java
animal.speak();
```
This line of code has no real meaning, though.  If an Animal is a purely abstract object, then its `speak` method will have no actual code.  At runtime, the program will dynamically dispatch to object's underlying type and call that object's `speak` method.  In other words, if the abstract `animal` object points to an instance of `Dog` class, then the above call may print `woof` to the screen.

In this case, the actual code called is determined by the underlying type of one abstraction.  One abstraction leads to a single dispatch.  The mechanism by which this (usually?) happens will be discussed in the procedural/imperative section.

# A Multiple Dispatch Example
At times it my be necessary to dispatch based on multiple types.

A programmer writing a game of rock-paper-scissors might use an abstraction of `Attacker` to represent a potential move.  This abstraction could prove useful throughout the program.  When the actual game is played, though, the type of both attackers will need to be known.  What we *want* to write is something along the lines of:
```java
(attacker1 and attacker2).determineWinner();
```
As an example, attacker1 might be resolved to `Rock` and attacker2 might be resolved to `Scissors`.  In this case, the code relating to rock beating scissors would be called.

# Multiple Dispatch in C
There are two main ways to go about this in C.  Both of these involve defining types which explicitly describe themselves.  In other words, an enum may be defined as:
```c
enum { ROCK, PAPER, SCISSORS} attacker;
```
The types for each attack could be defined by:
```c
struct { attacker TYPE = ROCK; }
struct { attacker TYPE = PAPER; }
struct { attacker TYPE = SCISSORS; }
```
## Big Switch Statement

The typically cringeworthy solution is to have a Big Switch Statement to handle every possible combination.  In psuedocode, for sanity related reasons:
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
This switch statement ends up being huge, even with only three types.

Beyond factoring each switch and/or case statement into its own function, the switch statement itself may be replaced with a jump table.

## Big Jump Table
The logic of a switch statement or chain of if/else statements based on explicitly defined types may be replaced by a jump table.  Before diving into the code, imagine a matrix of possible outcomes for the game itself:

- | Rock | Paper | Scissors |
- | - | - |
**Rock** | Tie | Lose | Win |
**Paper** | Win | Tie | Lose |
**Scissors** | Lose | Win | Tie |

We can translate this into code by creating functions for every possible combination of rock-paper-scissors, then populating a table based on the intersection of the types.  Dispatching to the correct function can then be done by looking up the function pointer in the table:
```c
BattleFunction generate_outcome = gameMatrix[attacker1.TYPE, attacker2.TYPE];
generate_outcome();
```
Maintaining the `TYPE` field in all of our objects is still a bit of a pain.  If we were to add or remove any types, we would have to edit our enum and update our matrix.

Introspection
=============
While finding a language or environment in which introspection is unavailable is not unheard of, reflection is commonly used to help alleviate some of the issues with the C-based approaches to this problem.

Instead of manually tracking each type, the underlying type of each abstraction is determined via a language's introspection features, and then the relevant code is dispatched to based on the findings.

As a side note, this approach is often times discovered independently and immediatelly touted as a panacea for the ugly switch/table based methods previously described, and the also ugly double indirection used in object oriented languages.  It is my opinion that it is not.  While it does simplify some aspects of the table-based approach, it still carries many of the drawbacks associated with that method, and it introduces many of its own drawbacks in the process.  These will be detailed in the general compare/contrast of each method toward the end.

# Object Oriented Approach
In object oriented languages, polymorphism is the go-to mechanism for replacing branches (be they switch or if) based on type.  To implement double dispatch, then, one can write something along the lines of:
```java
attacker1.handle(attacker2);
```
This dispatches to the appropriate `handle1 method based on attacker1's type.  If `attacker1` is a `rock`, for example, you could mentally translate this to:
```java
rock.handle(attacker2);
```
Since we're now in the Rock's `handle` method, we know we are a rock.  Now all that's left to do is figure out what attacker2 is.  We can do this in similar fasion inside of the rock.handle method:
```java
attacker2.handleRock(this);
```
We know that we're a `rock`, so `this` is really `rock`.  Again, if `attacker2` is **actually** `paper`, the call may be mentally translated to:
```java
paper.handleRock(rock)
```
And in our Paper class, the handleRock method can contain the code which handles the rock versus paper interaction.

### Header 3

> This is a blockquote.
>
> This is the second paragraph in the blockquote.
>
> ## This is an H2 in a blockquote
