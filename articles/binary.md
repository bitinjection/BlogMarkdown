Title: Spitting Headaches and Binary Searches
Author: Douglas Thompson
Date: 09-21-2015
Slug: splitting-binary-searches-1
Category: Programming
Summary: Is the midpoint formula good for a divide and conquer search?
Tags: Algorithms, Mathematics, Optimization, Programming

I want to put this out there now--I intend to follow this up at some point in the future with more significant insights as I still have a lot of questions about this topic.

Be sure to check out the [video](https://www.youtube.com/watch?v=2cXrsrPAbwk "Generatin combinations and permutations") if you have some time to kill!

# The Setup

I wrote a small website for combat probing in the game Eve Online.  Without going into much detail, there is at tool in the game to locate far away players called the Directional Scanner.  This tool works *sort of* like RADAR.  You specify a distance from your ship, click "Scan", and you receive a list of ships and other objects that are within that range.

For example, I might pick a range, say 7 AU (astronomical unit, one is about 150 million kilometers), and scan.  If there is a ship within 7 AU, it will be listed on my directional scanner. No additional information about it will be provided.  So we know there is a ship within 7 AU, but how can we quickly figure out *exactly* where it is in system?

The typical advice given here is to use a binary search by modifying the search distance on the directional scanner.  If a ship appeared on scan, we would then shorten our scan range to 3.5 AU and repeat the process.  On the other hand, if the ship appeared on scan, we might extend the distance to 10.5 AU (the maximum dscan range being 14AU, and assuming we knew the ship was in max scan range).  Using this technique, we can quickly zero in on our target.

Though it has changed in recent patches, it used to be that you had to type in the range of your scan *in kilometers*.  Manually doing math on and typing in 10 digit numbers can be time consuming (even when truncating results), error prone, and the number of "probes" that you have to do may mean the difference between life or death.  Thus, minimizing the number of iterations is vital.

# Problems Searching Space

I didn't realize until much later, but there is a problem with the approach of splitting the seach *range* in half each iteration.

The directional scanner searches a *space* within *x* kilometers of the ship.  The space it spans forms a sphere around the ship.  The thing is, *the radius of a sphere and its volume do no vary linearly*.

To illustrate this, let's take a simpler example and use a circle.  The radius of a circle is related to the area of a circle by the formula:

a = &pi; r ^ 2

Continuing with our concrete example, the area of a circle with radius 14 is 196&pi;, while the radius of a circle with radius 7 is 49&pi;.  In other words, we reduced our search area to one fourth instead of halving it!  Since cutting the search space in half is the basis for how the binary search actually works, this might be a problem.  In order to actually halve the search area, we need to take half of the *area*, not the radius, and then figure out what the radius for that given area is.  It seems obvious, but trust me when I say that nobody in Eve Online recognizes this problem.


If you would like a little bit more detail on this process, be sure to check out the associated [video](https://www.youtube.com/watch?v=2cXrsrPAbwk "Generatin combinations and permutations").

# Why Did This Happen?

There is actually a deeper problem here.  When we use the radius of the circle/sphere as our search criteria, the actual search space grows exponentially.  In other words, the search space is not really uniformly distributed.  This is an assumption that the binary search makes, but is this something that we can generally rely on?  And how much does it really matter?

As an anology that might be more relevant, let's take the *very common case* where we have some data that is distibuted normally instead of uniformly.  Suppose we have a bunch of SAT scores from high school students to search through.  If we wanted to find students with scores of 2100 we might use a binary search to do this.  There is a significant problem with this approach, though.  We assume the SAT's normally distributed scores are centered at a score of 1500.  If we make our first split at 1500, everything is great.  Half of the students are below or at 1500, the other half are above or at 1500.  Since we're searching for 2100, we'll hop to the middle of the upper half.  Though the upper half of the curve technically has no upper bound, the test itself tops out at 2400.  This makes our midpoint for the 1500-2400 range 1950.  And this is where the trouble starts.

The normal distribution for actual SAT scores might look like this: Mean 1500, Standard Deviation 300.  In such a case, SAT scores between 1500 and 1950 will comprise 86.64% of the remaining scores, while the scores between 1950 and 2400 will only contain 13.36%!  This isn't even close to what we were going for.

Though I have not messed with them, there are methods for transforming a normal distribution into a uniform one.  I suspect that this may be what you want to consider doing if you had, for instance, a normal distribution of data to search through.

# Back to Outer Space

For my small Eve Online tool, I realized that this sort of thing was causing problems.  I immediately changed the searching algorithm to the 3D version.  I soon realized that, despite Eve being a fully 3D game, many of the systems in game are essentially laid out on a 2D plane.  It was also often that I knew the direction of a particular target, and only wanted to find how far away they were--further reducing the dimensions of the problem to one.  As such, I stuck in 3 different options for performing the search:  one dimensional (line), two dimensional (circle), and three dimensional (sphere).

# Conclusions

Sadly, I am left with more questions than answers at this point.  While writing the program, I was happy with having the option to select the appropriate search criteria based on the problem at hand.  When writing this article, however, I suddenly am faced with finding answers of a more general nature.  I spent a little bit of time searching for other people's experiences with this problem, but ultimately came up with little insight.  I think that many people feel the binary search still finds a solution fast enough such that the overhead/complexity of optimizing the search is not worth it.  I do realize that this sort of thing can be seen as an unnecessary obsession over an optimization, but non uniform distributions are so common that I still feel like it's worth investigating--for science!

When I have more time, I will be sure to look into this more and write up a part 2, but of course if anyone has any knowledge they wish to impart upon me, be sure to do so!
