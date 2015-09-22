Title: Combinations without Replacement
Author: Douglas Thompson
Date: 09-20-2015
Slug: combinations-without-replacement
Category: Programming
Summary: A simple algorithm for generating combinations without replacement
Tags: Algorithms, Mathematics, Optimization, Data Structures

# Algorithm for Generating Combinations without Replacement

The algorithm this article is based around is nearly trivial, but it was something that tripped me up for a long time.  Thus, I feel compelled to write an article, but I also feel compelled to make it short (well, relatively speaking).

If you want a brief, but more technical overview, be sure to check out the associated [YouTube video](https://www.youtube.com/watch?v=LC4H1SF0Aug "Split points in binary searches")!  The simple algorithm and boring, abstract problem belie the much more widely applicable nature of the generalized problem.

# The Problem

Sometimes you have a collection of things, and you want to select each of the things from the collection, one at a time, in some random order.  You *don't* want to take the same element twice, however.  How do you end up handling this in an effective way?

# The Easy Soltion

One easy way to approach this problem is to simpy remove each element from the collection as you come across them, but which data structure would you use to do this?

You could use a resizable array, but each time you remove an element on average n/2 elements will need to be "shuffled down" in order to ensure the array is contiguous.  This can be an actual serious problem even on modern computers in many cases.

You could use a linked list, but then you'd have to transverse, on average, n/2 elements on average every iteration to find the item to remove.

A hash table might be suitable, but there's a good chance your data isn't in this format already, and you have to deal with the potentially substantial overheads associated with this.

# The Good Solution

It turns out that there is a really simple solution to this problem which has nearly no drawbacks with respect to performance or memeory.  It works like this:

1. Manually maintain an index into the array
2. Randomly (or however) select an element
3. Swap this element with the last one in the array
4. Decrement the index
5. Return array[index]

# Analysis

Should you use this algorithm?  I recently used it in a small program I wrote for finding scalping opportunities in the video game Eve Online.  But should I have?  This is a much harder question to answer.

## This algorithm is *dead simple*

The thing that strikes me the most about this algorithm is that it is insanely simple.  Simply removing the selected element from the list is nearly as difficult!  In fact, there is a commonly used C++ idiom, "swap and pop", which leverages this exact same mechanic for nearly the same reason.


## In today's world, this would usually be considered a vomit inducing micro-optimization

Most application programmers live in a world of immutable/persistent data structures and defensive copying.  The idea that an algorithm like this would ever see use in general is probably laughable to most programmers.  In particular, the program that I wrote would select an item at most once every 30 seconds.  Is this sort of optimization needed *at all*?  Obviously not.

Luckily, in my own side projects I get to do whatever I want!

Again, be sure to check out the associated [YouTube video](https://www.youtube.com/watch?v=LC4H1SF0Aug "Split points in binary searches") if you can stand it.  This algorithm address one solution to a larger class of problems which you probably encounter on a daily basis.
