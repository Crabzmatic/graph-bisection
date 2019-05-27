#ifndef GROWING_BISECTION_HPP
#define GROWING_BISECTION_HPP
#ifdef DEBUG
#define D if(true)
#else
#define D if(false)
#endif
#include <set>
#include <future>
#include <random>
#include <utility>
#include "../../ba-graph/include/impl/basic/include.hpp"
#include "../../ba-graph/include/impl/basic/graph.h"
#include <bisectionResult.hpp>
#include <iterativeBisection.hpp>
#include <bisectionUtilities.hpp>
using namespace ba_graph;

class ParallelBisectionResult : public BisectionResult{
public:
    explicit ParallelBisectionResult(int cutSize){
        this->cutSize = cutSize;
    }
    explicit ParallelBisectionResult(std::set<Vertex> bisection, int cutSize){
        this->bisection = std::move(bisection);
        this->cutSize = cutSize;
    }
};

inline void removeVertex(
        std::vector<Vertex> &unvisited,
        const Vertex &seed
        ) {
    for (unsigned int i = 0; i < unvisited.size(); ++i) {
        if (unvisited[i] == seed) {
            unvisited.erase(unvisited.begin() + i);
        }
    }
}

inline void insertNeighbors(
        std::set<Vertex> &toAdd,
        const RotationIterator &rot
        ) {
    for (auto &it : *rot) {
        toAdd.insert(it.v2());
    }
}

inline Vertex addMinimalGain(
        const Graph &G,
        std::set<Vertex> &bisection,
        std::set<Vertex> &toAdd,
        std::vector<Vertex> &unvisited,
        int &cutSize
        ) {
    Vertex addedVertex;
    int minimalGain = INT32_MAX;
    for (const auto &v : toAdd) {
        auto bsc_temp = bisection;
        bsc_temp.insert(v);
        int gain = findCutSize(G, bsc_temp) - cutSize;
        if (minimalGain > gain) {
            minimalGain = gain;
            addedVertex = v;
        }
    }
    toAdd.erase(addedVertex);
    bisection.insert(addedVertex);
    removeVertex(unvisited, addedVertex);
    cutSize += minimalGain;
    return addedVertex;
}

inline void updateToAdd(
        const Graph &G,
        const std::set<Vertex> &bisection,
        std::set<Vertex> &toAdd,
        const Vertex &addedVertex
        ) {
    auto r = G.find(addedVertex);
    auto adjacent = std::set<Vertex>();
    insertNeighbors(adjacent, r);
    for (const auto &v : adjacent) {
        if (toAdd.count(v) == 0 && bisection.count(v) == 0) {
            toAdd.insert(v);
        }
    }
}

inline int addBestVertex(
        const Graph &G,
        std::set<Vertex> &bisection,
        std::set<Vertex> &toAdd,
        std::vector<Vertex> &unvisited,
        int cutSize
        ) {
    std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
    if (toAdd.empty()) {
        std::uniform_int_distribution<> dist(0, unvisited.size() - 1);
        toAdd.insert(unvisited[dist(gen)]);
    }
    Vertex addedVertex = addMinimalGain(G, bisection, toAdd, unvisited, cutSize);
    updateToAdd(G, bisection, toAdd, addedVertex);
    return cutSize;
}

inline ParallelBisectionResult growBisection(
        const Graph &G,
        const Vertex &seed
        ){
    auto bisection = std::set<Vertex>{seed};
    auto toAdd = std::set<Vertex>();
    auto unvisited = std::vector<Vertex>();
    for (auto &it : G){
        unvisited.push_back(it.v());
    }
    removeVertex(unvisited, seed);
    RotationIterator rot = G.find(seed);
    insertNeighbors(toAdd, rot);
    unsigned long long limit = graphHalf(G);
    int cutSize = findCutSize(G, bisection);
    while(bisection.size() < limit){
        cutSize = addBestVertex(G, bisection, toAdd, unvisited, cutSize);
    }
    return ParallelBisectionResult(bisection, cutSize);
}

inline void setGrowBisectionResult(
        std::promise<ParallelBisectionResult> && promise,
        const Graph &G,
        const Vertex &seed
        ) {
    auto result = growBisection(G, seed);
    return promise.set_value(result);
}

inline BisectionResult getMinimalResult(
        const Graph &G,
        bool doIterative,
        std::vector<std::future<ParallelBisectionResult>> &futures
        ) {
    BisectionResult *minimal = new ParallelBisectionResult(INT32_MAX);
    for (auto & future : futures) {
        auto res = future.get();
        if (doIterative) {
            auto bsc = res.getBisection();
            auto res_r = iterateBisection(G, bsc);
            res.setBisection(res_r.getBisection());
            res.setCutSize(res_r.getCutSize());
        }
        if (minimal->getCutSize() > res.getCutSize()) {
            minimal->setBisection(res.getBisection());
            minimal->setCutSize(res.getCutSize());
        }
    }
    return *minimal;
}

inline BisectionResult doGrowingBisection(
        const Graph &G,
        bool doIterative
        ) {
    auto futures = std::vector<std::future<ParallelBisectionResult>>();
    runAsParallel(G, setGrowBisectionResult, futures);
    return getMinimalResult(G, doIterative, futures);
}
#endif