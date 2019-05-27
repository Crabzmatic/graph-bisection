#ifndef COMBINED_BISECTION_HPP
#define COMBINED_BISECTION_HPP
#include <bisectionResult.hpp>
#include <growingBisection.hpp>
using namespace ba_graph;

BisectionResult doCombinedBisection(const Graph &G){
    return doGrowingBisection(G, true);
}
#endif
