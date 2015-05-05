# Flipgraph

Flipgraph is a program for generation of flip graphs for triangulations. It is part of my Master's thesis.

## Usage

The program has several modes that can be specified via the `-m` command line parameter. For instance, the command ` flipgraph -m generate -n 7` generates the flip graph on all triangulations with 7 vertices. The diameter of the same flip graph is computed by `flipgraph -m diameter -n 7`. Moreover, `flipgraph -m triangulation -n 7 -i 2` will output the second triangulation discovered during the exploration of the flip graph. Since the flip graph is explored using a breadth first search, the triangulations are ordered according to their distance to the canonical triangulation. This ordering can be reversed by adding the `-r` flag. That is, `flipgraph -m triangulation -n 7 -r -i 1` will output a triangulation that has maximal distance to the canonical triangulation.

## What is a Flip Graph?

A maximal planar graph is called a *triangulation* since all of its faces are triangles.
We consider abstract unlabeled triangulations, that is, we do not distinguish between triangulations in the same isomorphism class.
Any edge *{u,v}* in a triangulation *T* has two incident triangles, say *uva* and *vub*.
A *flip* operation consists of removing the edge *{u,v}* and then inserting the edge *{a,b}* instead.
However, the edge *{u,v}* is *flippable* only if the edge *{a,b}* is not already present in *T*.
The induced *flip graph* *F = (V,E)* is the graph where *V* is the set of all triangulations on *n* vertices.
Moreover, for all triangulations *T* and *T'*, the edge *{T,T'}* is contained in *E* if and only if *T* can be transformed into *T'* by a single flip.
