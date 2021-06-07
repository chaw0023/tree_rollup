# The Tree Rollup Problem
This repository has code that emulates a tree rollup problem. 
In a generalized tree, values (weights) can only be added to the leaf of the tree. For any leaf, the newly added leaf weight must propagate upstream to all the ancestors, all the way to root.
This happens for all leaf node weight additions.

## scalar
Rollup of values on a scalar CPU machine

## SIMD
Rollup of values using vectorized SIMD code (in progresss)
