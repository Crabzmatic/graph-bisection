# graph-bisection
Algorithms for graph partitioning approximation. The goal is to find a set of vertices with minimal cut size, i.e. with minimal number of edges connecting 2 vertices not belonging to the same _half_. Bisection set size must be half of the number of vertices in graph.

Made with _ba-graph_ library (https://bitbucket.org/relatko/ba-graph). Relative path should be:

project/
-  ba-graph/
-  graph-bisection/

Function _doIterativeBisection()_ returns a set of vertices that belong to the bisection and does this using something I called "Single vertex exchange iterative" bisection method.

Function _doGrowingBisection()_ returns a set of vertices that belong to the bisection and does this using "Parallel growing bisection" which selectes a number of vertices (based on hardware concurrency) at random and starts spreading around them until there are half of the vertices in the set. This is done on separate threads.
