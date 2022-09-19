# codingame-blunder3
C++ Solution to the [Blunder 3 puzzle on Codingame](https://www.codingame.com/ide/puzzle/blunder-episode-3)

We are given a bunch of (n,t) coordinates corresponding to the runtime of a particular algorithm given input size n. We are told find the best fitting algorithmic complexity (out of a few potentital candidates). For each complexity function f, the solution finds C such that Cf has minimal quadratic error when approximating the input (C can be determined arbitrarily close to the true minimum). 

An improvement to the solution would be to linearize everything. Plot t against nlogn and check how close we are to a straight line, rather than plotting t against n and checking how close we are to nlogn, etc.
