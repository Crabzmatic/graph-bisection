#ifndef ITERATIVE_BISECTION_HPP
#define ITERATIVE_BISECTION_HPP
#ifdef DEBUG
    #define D if(true)
#else
    #define D if(false)
#endif
#include <set>
#include <iostream>
#include "../../ba-graph/include/impl/basic/include.hpp"
#include "../../ba-graph/include/impl/basic/graph.h"
#include "bisectionResult.hpp"

using namespace ba_graph;
struct MinBsc {
    std::set<Vertex> minBsc;
    int minCutSize;
    int minCutSizeDiff;
    MinBsc(){
        minBsc = std::set<Vertex>();
        minCutSize = 0;
        minCutSizeDiff = 0;
    }
};
struct Change{
    Vertex vertexChanged;
    bool changedSomething;
    std::set<Vertex> v_changed;
    Change(){
        v_changed = std::set<Vertex>();
        changedSomething = false;
    }
};
class IterativeBisectionResult : public BisectionResult{
public:
    IterativeBisectionResult(){
        this->bisection = std::set<Vertex>();
        this->cutSize = 0;
    }
    IterativeBisectionResult(std::set<Vertex> &_bisection, int _cutSize){
        this->bisection = _bisection;
        this->cutSize = _cutSize;
    }
};
inline int findCutSize(
        const Graph &G,
        const std::set<Vertex> &partition
) {
    int cutSize = 0;
    for (auto &rot : G) {
        if (partition.count(rot.v()) > 0) {
            for (auto &inc: rot) {
                if (partition.count(inc.v2()) == 0)
                    cutSize++;
            }
        }
    }
    return cutSize;
}
inline MinBsc addVertexInBisection(
        const Graph &G,
        const std::set<Vertex> &bisection,
        int cutSize,
        Change &adding
) {
    MinBsc minBsc = MinBsc();
    minBsc.minCutSize = cutSize;
    adding.changedSomething = false;
    for (auto &rot : G) {
        if (bisection.count(rot.v()) == 0 && adding.v_changed.count(rot.v()) == 0) {
            auto bisection_temp = bisection;
            bisection_temp.insert(rot.v());
            int cutSize_temp = findCutSize(G, bisection_temp);
            int diff = cutSize - cutSize_temp;
            if (cutSize_temp <= minBsc.minCutSize && diff >= 0) {
                adding.changedSomething = true;
                adding.vertexChanged = rot.v();
                minBsc.minBsc = bisection_temp;
                minBsc.minCutSize = cutSize_temp;
                minBsc.minCutSizeDiff = diff;
            }
        }
    }
    return minBsc;
}
inline BisectionResult removeVertexInBisection(
        const Graph &G,
        const std::set<Vertex> &bisection,
        int cutSize,
        Change &removing,
        MinBsc &minBsc
) {
    BisectionResult *newBsc = new IterativeBisectionResult();
    newBsc->setCutSize(cutSize);
    removing.changedSomething = false;
    for (auto &rot : G) {
        if (bisection.count(rot.v()) > 0 && removing.v_changed.count(rot.v()) == 0) {
            auto bisection_temp = minBsc.minBsc;
            bisection_temp.erase(rot.v());
            int cutSize_temp = findCutSize(G, bisection_temp);
            if (cutSize_temp <= minBsc.minCutSize + minBsc.minCutSizeDiff && cutSize_temp <= newBsc->getCutSize()) {
                removing.changedSomething = true;
                removing.vertexChanged = rot.v();
                newBsc->setBisection(bisection_temp);
                newBsc->setCutSize(cutSize_temp);
            }
        }
    }
    return *newBsc;
}
inline void fillBisection(const Graph &G, std::set<Vertex> &bisection) {
    int inserted  = 0,
        half = G.order() % 2 == 0 ? G.order() / 2 : (G.order() + 1) / 2;
    for (auto &rot : G) {
        if (inserted < half) {
            bisection.insert(rot.v());
            inserted++;
        } else break;
    }
}
inline BisectionResult iterateBisection(const Graph &G, std::set<Vertex> &bisection) {
    int cutSize = findCutSize(G, bisection);
    D std::cout<< "Process started with cut size: " << cutSize << std::endl;
    Change removing = Change();
    Change adding = Change();
    while (true) {
        MinBsc minBsc = addVertexInBisection(G, bisection, cutSize, adding);
        if (!adding.changedSomething){
            D std::cout << "Did not find any suitable Vertex to add. Cut size is: " << cutSize << std::endl;
            break;
        }
        D {
            if (minBsc.minCutSize < cutSize) {
                std::cout << "Vertex was added that reduced cut size to: " << minBsc.minCutSize << std::endl;
            } else {
                std::cout << "Vertex was added. Cut size is still: " << minBsc.minCutSize << std::endl;
            }
        }
        BisectionResult newBisection = removeVertexInBisection(G, bisection, cutSize, removing, minBsc);
        if (!removing.changedSomething) {
            D std::cout << "Did not find any suitable Vertex to remove. Will not consider adding the same Vertex in next iteration." << std::endl;
            adding.v_changed.insert(adding.vertexChanged);
            continue;
        }
        if (adding.vertexChanged == removing.vertexChanged){
            D std::cout << "Tried to remove the Vertex that was added. Will not consider removing this Vertex in next iteration." << std::endl;
            removing.v_changed.insert(removing.vertexChanged);
            continue;
        }
        if (newBisection.getCutSize() == cutSize) {
            D std::cout << "Cut size did not change after this iteration. Will not consider adding the same Vertex in next iteration." << std::endl;
            adding.v_changed.insert(adding.vertexChanged);
            removing.v_changed.clear();
            continue;
        }
        cutSize = newBisection.getCutSize();
        bisection = newBisection.getBisection();
        adding.v_changed.clear();
        removing.v_changed.clear();
        D std::cout << "Vertex removed. Successfully reduced cut size to: " << cutSize << std::endl;
    }
    return *(new IterativeBisectionResult(bisection, cutSize));
}
inline BisectionResult doIterativeBisection(const Graph &G) {
    auto bisection = std::set<Vertex>();
    fillBisection(G, bisection);
    return iterateBisection(G, bisection);
}
#endif