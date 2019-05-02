# graph-bisection
Algorithm for graph partitioning approximation. The goal is to find a set of vertices with minimal cut size, i.e. with minimal number of edges connecting 2 vertices not belonging to the same _half_. Bisection set size must be half of the number of vertices in graph.

Made with _ba-graph_ library (https://bitbucket.org/relatko/ba-graph). Relative path should be:

project/
-  ba-graph/
-  graph-bisection/

Function _doBisection()_ returns a set of vertices that belong to the bisection.
This function was done for cubic graphs, mainly.
