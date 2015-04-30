# Flipgraph

Flipgraph is a program for generation of flip graphs for triangulations. It is part of my Master's thesis.

## Usage

Coming soon...

## What is a Flip Graph?

A maximal planar graph is called a *triangulation* since all of its faces are triangles.
We consider abstract unlabeled triangulations, that is, we do not distinguish between triangulations in the same isomorphism class.
Any edge *{u,v}* in a triangulation *T* has two incident triangles, say *uva* and *vub*.
A *flip* operation consists of removing the edge *{u,v}* and then inserting the edge *{a,b}* instead.
However, the edge *{u,v}* is *flippable* only if the edge *{a,b}* is not already present in *T*.
The induced *flip graph* *F = (V,E)* is the graph where *V* is the set of all triangulations on *n* vertices.
Moreover, for all triangulations *T* and *T'*, the edge *{T,T'}* is contained in *E* if and only if *T* can be transformed into *T'* by a single flip.
