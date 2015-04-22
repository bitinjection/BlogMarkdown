Title: Writing a Trade Finder for Eve
Author: Douglas Thompson
Date: 04-22-2015
Slug: eve-marketing-tool
Category: Programming

Finding securities to scalp in Eve essentially boils down to tedious, repetative work.  Luckily, the game provides a good toolset for developers, so I set out to write a program to save some time and effort.

# Spreadsheets in Space

In another article, I discussed an approach I sometimes take when trading in Eve Online.   A formula for any scalper would regularly use is the one which computs the **bid-ask percentage**:

<a href="http://www.codecogs.com/eqnedit.php?latex=100\frac{a-b}{b}" target="_blank"><img src="http://latex.codecogs.com/gif.latex?100\frac{a-b}{b}" title="100\frac{a-b}{b}" /></a>

I used this formula so many times that I typically mentally estimate the bid-ask spread on a security in under a second or two.

There are other calculatable numbers to consider when determining whether or not to invest in a security, and it can be slow and annoying to calculate each of them for every possible candidate--there are literally thousands.

A good tool to help perform many of the desired calculations quickly is the spreadsheet.  A spreadsheet can be constructed relatively quickly, and the amount of time and effort it saves is substantial.  Spreadsheets are so useful in so many Eve activities, thatt the game is sometimes called "Excel Online" or "Spreadsheets in Space".  Thus, I knew I was becoming a real Eve pro once I began to toss together spreadsheets.

Sadly, the convenience of spreadsheets only goes so far.  Searching through the thousands of securities, copying and pasting in each bid/ask, and so on, is still massively annoying, slow, and repetitive.

# Eve API

One of the great things about Eve Online is its support for developers.  In fact, many would argue that it is nearly impossible to play Eve without third part tools.  The programs EFT/Pyfa and EveMon are a virtual requirement to play, while tools such as Dotlan, Tripwire, Eve Central, and so on are familiar to nearly any player.

Getting market data from Eve is as simple as fetching the it via a URL.  The data is provided in JSON, and from there it's as simple as tossing the numbers into whatever formulas you desire and displaying the data on the screen.

Really, making a tool to do this is so simple that it is not even really worth writing about.

# Thorttling

What made me want to write this article was how I decided to go about polling the API for data.  I am confident that there are better ways to go about doing this, but I though I would share what I came up with.  Any opinions and criticisms would be welcome.  Dragons ahead!

Like many APIs these days, CCP has limited the number of queries it will allow clients to make.  In a perfect world, I'd simply ask the server for all orders of all securities all over New Eden every few seconds or so, and I'd work with that data.  I cannot do this, though, due to the restrictions on the use of the API.  So how do I go about eventually getting all the data, and how do I continue to update prices as the application runs in a way that would best benefit me as a trader?

# Requirements

There are two basic things I was looking for.

* Need to consider the entire market
* Need to spend more time fetching prices for securities that I actually care about

These two ideas actually compete to an extent.  I want to know what the spread is of **everything**, but at the same time, I don't want to be constantly fetching prices for things I don't care about.  I still want to fetch prices for those things **sometimes**, since they may eventually **become** something I care about, but, in general, I don't want to spend my precious price-fetching-quota on things that are usually not interesting.

## Scheduling Algorithms

My first thought was to turn to the scheduling algorithms that operating systems use to determine how to divy up CPU time.  I figured this would be a slam dunk, but since we are talking about programming, it was, of course, not that simple.

Scheduling algorithms **seem** like a good fit because they cycle through a list of jobs, and the jobs themselves are often given different priorities, or importance.

Scheduling algorithms end up being an awkward fit because they seek to address a slightly different poblem.  In general, scheduling algorithms are expecting each "job" to take longer than a quantum, so they focus on sifting through a compartively (?) small number of long running jobs.  

In our case, we actually have a huge number of short jobs that we can only run every once in a while, so many of the algorithms don't make much sense for us.  Our "jobs" will never actually finish, and slowly iterating through the entire list would take a very long time.  I'm not saying that the typical schedulig algorthims don't fit at all, but they weren't really what I was looking for.  There was one, though, which caught my eye.

## Playing the Lottery

The lottery-based scheduling algorithm is so simple and dumb.  You simply assign a number of "tickets" to each job, then pick one of them randomly.  That job gets to run for a bit, and then the process repeats.  The reason this approach appeals to me is because it mimics what I actually do.

When I find a security that I have traded successfully before, I'll trade it until it becomes unprofitable, but I'll randomly check back to see if the bid-ask spead has re-emerged.  If this happens often with a particular security, I'm more likely to check on it regularly.  If a security seems to always be a poor choice, such as with PLEX, I rarely, if ever check on it--but I'll still come back now and again and check on it just for giggles.  For other securities, I sometimes run through the listings on the market and randomly check out various things to see how their prices are doing.

# Scoring

If I use the above lottery system and give each security a single ticket, then the Random Number Gods will likely eventually hit on every security.  If that's all I was interested in doing, though, I could just run through the list in alphabetical order and be done.

I want to adjust the number of tickets that each secuirty gets based on how interested I am in recalculating the spread and other metrics that I use when trading.  I looked at my trading habits and realized a few things:

* Some securities I mostly ignore as they are usually bad
* Some securities I trade a lot, but I almost do this blindly because they're always good
* Some securities are good sometimes, and not good other times.

I am not too concerned with the securities in the first two bullets.  These items are usually always good or always bad, and, as a result, I'm not usually having to plug their numbers into any spreadsheets.

The last bullet is important, though.  These items often jump the fence between being good to trade and bad to trade.  One of the main reasons a security does this is because people will notice a big spread on a semi-popular security and begin to invest in it.  Because this is a semi-popular security, "regular" players will often buy and sell them and the scalpers involved can make a fair bit of dosh from them.  The popularity of the item, though, causes more competition from other traders to quickly emerge, and the security's spread collapses quickly.  Traders lose interest, the spread begins to grow again, and the process repeats.  **These are the securities I want to check in on regularly**.  I still want to fetch prices for the less interesting securities "just in case", but I want to fetch the prices for these interesting ones more often.

## How Do?

If a security consistently displays "good" numbers, or consistently displays "bad" numbers, I want it to be checked less and less.  If a bad security begins to show good signs, or if a security constantly bounces between "good" and "bad", I want those to be checked more frequently.  I want to make sure, though, that a security always has at least **some** chance of being checked.

Ideally, I would quickly push the securities which show consistent behavior off to the sides, but I don't want to slow down this "pushing off to the sides" as they move away from the center.  Again, I never want to push them off of the table, just a little bit further away each time.

When thinking about this, I came up with two similar solutions.  I'll start with the more obvious, sensible one first, then discuss the first one that popped into my head second.

## The Normal Approach

I have no idea why this did not occur to me first, but the easy solution is also the Big One. The Grand Daddy Curve of curves.  The Bell Curve.

The standard normal bell curve works well for my purposes.  Each security can be given a single ticket, as their prices is checked, the ticket is adjusted to be some value along the standard normal bell curve.  For instance, a "good" security would move to the right, while a "bad" security would move to the left.  As the security receieves repeated "good" or repeated "bad" ratings, it is selected for evaluation less frequently.  Securities which regularly switch back and forth between "good" and "bad" remain closer to the middle of the curve and are selected for re-evaulation more frequently.

"Good" securities, of course, will still be displayed in the program for trading, but they are not frequently reevaluated until they have shown at least a little bit of consistency.  This eliminates the problem of wasting API calls on securities which barely crossed the line into being a "good" security for a moment, only before regressing to a "bad" one.

I should also note that the user has the ability to manually request a security be updated.  The above approach is mainly for updating prices over time "in the background".
