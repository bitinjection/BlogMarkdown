Title: Trading for Space Bucks in Eve 101
Author: Douglas Thompson
Date: 04-21-2015
Modified: 04-21-2015
Slug: state-pattern
Category: Gaming

As far as games go, there is none which has created a more realistic and stable economy than Eve Online.  The marketing in Eve is what originally drew me to the game over two years ago, and I have finally decided to write a tool to help me with my trade decisions.  In this article, though, I will explains what Eve is and the basic approach one might take to trading within the game.  I'll save the math and programming for a seperate post.

This article is **long** and primarily covers the basics of Eve Online, markets, and some very basic financial math and ideas.  I feel that most people will find this all boring and pointless.  Part 2 may be a bit more interesting (though probably still mostly pointless).

I want to spend some space explaining what Eve is and why its economy works so well, as well as how the game makes it easy to profit from various professions (such as trading)

# Eve Online
Eve Online is a "massively multiplayer science fiction space sandbox" game.  The game is known for its impressive and massive space battles, deep political scene, scams and backstabbing, griefing, and so on. Longtime players will likely tell you it is less of a game more of a "sociopath simulator".

## Greifers Online
`
One of the main reasons Eve's universe seems so unforgiving is that the loss in the game, unlike most games, is permenant.  Playes in most persistent-world games are happy to spend several hours a day for months on end mindlessly "grinding" for loot, gear, or whatever else, knowing that they will forever own whatever the gather.  In Eve, things are yours only until they are destroyed, stolen, or sold.  After that, just like in Real Life, they are gone forever.  Thus, players of Eve hold tightly onto their wallets and treat ISK, the name of the in-game currency, like it is water in a desert.  The most common piece of advice doled out to newcomers is "Don't fly what you can't afford to lose".  Piracy, suicide bombings, backstabbing, scamming, etc, are all commonplace in the pursuit of this always-valuable space money.

## Faucets and Sinks
The problem introduced with classical persistent-world games is that gold and items are generated out of nothing and are never removed from the game.  For instance, if a player kills a wolf, the wolf may drop a "Wolf Skin" item.  Wolves will appear endlessly for players to kill, so players will repeatedly kill wolves until everyone has more than enough "Wolf Skin"s.  

While resources are still magically injected into the game in Eve Online, the rate at which they are generated is offset by the rate at which they are destroyed.  Thus, a pilot in Eve is never endlessly rich--they most always have a source of income.  Similarly, items and the currency of the game never inflate or deflate unless due to economic pressure (or game patches :x).

## The Market

Another big difference in the way resources are handled in Eve is the way in which the market works.  In a typical game, can find non-player characters which will buy everything you have at a set prices, and thse vendors will generate gold out of nothing to pay for it.  In Eve Online, the vast majority of goods will only ever change hands between players.  If you want to sell something, you better hope there is someone out there who wants to buy it from you.

In traditonal games, items are often bought and sold from non-player characters, or NPCs.  For the most part, this does not happen in Eve.  In Eve, if you have an item that you would like to sell, you have to sell it to another player.  Likewise, if you want a particular items, the only way to buy it is often from another player.

## You Gotta Have Skills

Lastly, Eve Online has a complex skill system in which your character passively trains abilities over time.  These skills do many things.  Market related skills, for example, might allow a character to place orders remotely, create leverage, place more buy/sell orders than other players, get a break on taxes and broker fees, and so on.  The last two in particular will becomes somewhat important for this article.

# Markets (in general)

## Buy and Sell Orders

Suppose Alice has a hotdog to sell.  Alice wants to sell her hotdog for $2.  The problem is, nobody wants to buy it *right now*.  She is pretty sure that someone will want to buy it for $2 eventually, so what does she do?

She creates a sell order.  What this does is list her hotdog on the market for $2.  If anybody comes by and decides they want a hotdog, they can check the market and see her hotdog listed for $2.  If they want to buy it, they pay the $2 to Alice and get her hotdog.

The opposite works as well.  Suppose Bob wants to buy a hotdog for $1.  Bob checks the market for sell orders, but all he sees is a sell order for a ridiculous $2.  He would really like a hotdog eventually, so he places a buy order for $1.  If someone comes by with a hotdog to sell, and they think that $1 is a fair price, they can fulfill Bob's buy order by giving him a hotdog and receiving the $1.

## Scalping and the Bid-Ask Spread

Suppose that both Alice and Bob have their sell and buy orders listed on the market.  Chuck checks out the hotdog listings and sees that Alice is selling a hotdog for $2, and that Bob wants to buy a hotdog for $1.

Check notices a "gap" between Bob's buy order and Alice's sell order of $1.  This "gap" is called the bid-ask spread (buy orders being bids, sell orders being asks).

Chuck has an epiphany--if he places a buy order that is just slightly more than Bob's, he will be able to buy a hotdog for cheap.  If he then immediately relists the hotdog in a sell order at just under Alice's price, he can potentially make a profit for doing nearly nothing.

For instance, if Chuck places a buy order at $1.01, the next person to come along and sell a hotdog will sell it to the highest bid, Chuck's $1.01.  Check can then relist the hotdog at $1.99, and the next person to come along and buy a hotdog will buy Chuck's, netting Chuck an easy $0.98.

Mathematically, Chuck invested $1.01 and earned $0.98, for a near 100% increase in capital.

As more people notice this large bid-ask spread, they will move into the market and begin competing with Chuck.  As such, the constant out-bidding and undercutting will cause the bid-ask spread to collapse.  As it does, people will lose interest in this item, or **security**, and the bid-ask spread may re-emerge later on.

Taking advantage of the spread to profit in this way is referred to as **scalping**.

## Liquidity and Volume

A particular security in Eve may have an enormous bid-ask spread, but it may only be because nobody has shown an interest on either end.  For instance, an item may be very rare, but provide only minor benefits over more common, similar choices. The seller does not want to sell their rare item for cheap, and buyers do not want to pay much for an item that offers little advantage.  Such an item would be said to have low liquidity.  

Another, related term is "volume".  Eve Online counts the number of times a security is successfully traded in a day.  This number is known as the "volume" of a security.

# Math

## Volume versus Bid-Ask Spread

It is important to realize that volume can compensate for a poor bid-ask spread.

For instnace, if you find an security with a bid-ask spread of 15 million, and you can flip one security per day, you will profit 15 million from that security.

If you instead find a security with a bid-ask spread of only 5 million, but you can flip that security 3 times during a day, you will **still profit 15 million**.

The general idea is that you want to consider the product of the volume and the bid-ask spread in order to determine how much you might stand to profit from flipping a security.

## Fees and Bid-Ask percent

A popular item in Eve Online is the 30 Day Pilot License Extension (PLEX).  A PLEX will grant a player 30 days of game time.  They sell for $20 USD, or, currently, about 838 million ISK.  At the time of writing, the highest buy order for a PLEX is 833 million ISK.  The liquidity of this item is not too bad, so you may think that you could flip a few of them daily.  The bid-ask spread is 5 million, so if you could flip 10 per day, you might think that you could profit 50 million is per day.

The problem is that Eve taxes all sales, and imposes brokers fees on all buy/sell orders placed (modified by skills and standings).  If, for instance, your brokers fees and taxes were both %0.75, you would end up paying over 20 million ISK in fees per "flip", costing you much more than the 5 million that you earn.

Thus, it is important to factor taxes and fees into the trade.

Since taxes and fees are a percentage of the item's value, it may help to look at the bid-ask spread as a percentage of the item's value as well.  Thus, a good bid-ask **percentage** indicates that we will make money even if the value of the item is high, regardless of however much we may lose to taxes and fees.

For instance, in the above example the bid-ask spread is 5 million, but the bid-ask percentage is only %0.6.

If we consider another item, the Federation Navy Stasis Webifier, it currently sells for roughly 55 million ISK and buy for 50 million ISK.  The ask-bid spread is also 5 million, but the ask-bid percentage is 10%.  Taxes and fees end up costing us a little over 1 million ISK, but we profit about 4 million isk everytime we flip one of these--**much** better than losing 15 million per flip like we would have with the PLEX.

## Volatility

Volatility refers to the tendency for a product to move significantly in price.  For scalping, volatility is generally A Bad Thing.  In the previous example, a trader might buy a Federation Navy Stasis Webifier at 50 million, expecting it to sell at 55 million.  If the buy order only fills because the market has suddenly become saturated with Federation Navy Webs, the price of the webs will rapidly fall.  Thus, the web itself may not be sellable for 55 million as anticipated, and the trader may have to take a loss.

As I have played Eve for a couple of years, I have a general understanding of what may or may not be volatile, so I do not take this into account and instead defer to my own judgement.  Additionally, volatility in most securities in Eve relately heavily to external factors, such as coalition movements and game patches--things which can not be easily calculated progmatically.

# Conclusions

This very long article offers some background and motivation for the otherwise simple problem of finding quick and easy trades in Eve Onlne.  The main ideas are

* Make sure the bid-ask precentage is good
* Make sure the item has good liquidity versus the bid-ask spread

For instance, a weak bid-ask percentage (but still positive after fees) but high liquidity might be a good trade, and good bid-ask percentage at a low, but still reasonable liquidity might be a good trade.  And, of course, a good bid-ask percentage trade with high liquidity would probably be great.

We shift gears in part 2 and examine how to go about writing a program to seek out these desirable trades.  We will focus on the actual math behind finding a security to scalp, and some probably awful ideas on how to determine which the program should pull data for.
