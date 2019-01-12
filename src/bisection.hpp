#ifndef BISECTION_HPP
#define BISECTION_HPP

#include <set>
#include <iostream>
#include "../../ba-graph/include/impl/basic/include.hpp"
#include "../../ba-graph/include/impl/basic/graph.h"

using namespace ba_graph;

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

void addVertexInBisection(
        const Graph &G,
        const std::set<Vertex> &bisection,
        int cutSize,
        const std::set<Vertex> &excludedAdd,
        Vertex &added,
        int &minCutSize,
        int &minCutSizeDiff,
        std::set<Vertex> &minBisection,
        bool &addedSomething
) {
    minCutSize = cutSize;
    minCutSizeDiff = 0;
    minBisection = std::set<Vertex>();
    addedSomething = false;

    for (auto &rot : G) {

        if (bisection.count(rot.v()) == 0 && excludedAdd.count(rot.v()) == 0) {
            auto bisection_temp = bisection;
            bisection_temp.insert(rot.v());

            int cutSize_temp = findCutSize(G, bisection_temp);
            int diff = cutSize - cutSize_temp;

            if (cutSize_temp <= minCutSize && diff >= 0) {
                addedSomething = true;
                added = rot.v();
                minBisection = bisection_temp;
                minCutSize = cutSize_temp;
                minCutSizeDiff = diff;
            }
        }
    }
}

void removeVertexInBisection(
        const Graph &G,
        const std::set<Vertex> &bisection,
        int cutSize,
        const std::set<Vertex> &excludedRemove,
        int minCutSize,
        int minCutSizeDiff,
        const std::set<Vertex> &minBisection,
        Vertex &removed,
        int &newCutSize,
        std::set<Vertex> &newBisection,
        bool &removedSomething
) {
    newCutSize = cutSize;
    removedSomething = false;
    for (auto &rot : G) {

        if (bisection.count(rot.v()) > 0 && excludedRemove.count(rot.v()) == 0) {
            auto bisection_temp = minBisection;
            bisection_temp.erase(rot.v());

            int cutSize_temp = findCutSize(G, bisection_temp);

            if (cutSize_temp <= minCutSize + minCutSizeDiff && cutSize_temp <= newCutSize) {
                removedSomething = true;
                removed = rot.v();
                newBisection = bisection_temp;
                newCutSize = cutSize_temp;
            }
        }
    }
}

void fillBisection(const Graph &G, std::set<Vertex> &bisection) {
    int inserted  = 0,
        half = G.order() % 2 == 0 ? G.order() / 2 : (G.order() + 1) / 2;
    for (auto &rot : G) {
        if (inserted < half) {
            bisection.insert(rot.v());
            inserted++;
        } else break;
    }
}

std::set<Vertex> iterateBisection(const Graph &G, std::set<Vertex> &bisection) {

    int cutSize = findCutSize(G, bisection);
    std::cout<< "Process started with cut size: " << cutSize << std::endl;

    Vertex removed, added;
    auto excludedAdd = std::set<Vertex>();
    auto excludedRemove = std::set<Vertex>();

    while (true) {
        int minCutSize;
        int minCutSizeDiff;
        std::set<Vertex> minBisection;
        bool addedSomething;

        addVertexInBisection(G, bisection, cutSize, excludedAdd, added, minCutSize,
                             minCutSizeDiff, minBisection, addedSomething);

        if (!addedSomething){
            std::cout << "Did not find any suitable Vertex to add. Cut size is: " << cutSize << std::endl;
            break;
        }
        if (minCutSize < cutSize) {
            std::cout << "Vertex was added that reduced cut size to: " << minCutSize << std::endl;
        } else {
            std::cout << "Vertex was added. Cut size is still: " << minCutSize << std::endl;
        }

        int newCutSize;
        std::set<Vertex> newBisection;
        bool removedSomething;

        removeVertexInBisection(G, bisection, cutSize, excludedRemove, minCutSize, minCutSizeDiff, minBisection,
                                removed, newCutSize, newBisection, removedSomething);

        if (!removedSomething) {
            std::cout << "Did not find any suitable Vertex to remove. Will not consider adding the same Vertex in next iteration." << std::endl;
            excludedAdd.insert(added);
            continue;
        }
        if (added == removed){
            std::cout << "Tried to remove the Vertex that was added. Will not consider removing this Vertex in next iteration." << std::endl;
            excludedRemove.insert(removed);
            continue;
        }
        if (newCutSize == cutSize) {
            std::cout << "Cut size did not change after this iteration. Will not consider adding the same Vertex in next iteration." << std::endl;
            excludedAdd.insert(added);
            excludedRemove.clear();
            continue;
        }
        cutSize = newCutSize;
        bisection = newBisection;
        excludedAdd.clear();
        excludedRemove.clear();
        std::cout << "Vertex removed. Successfully reduced cut size to: " << cutSize << std::endl;
    }
    return bisection;
}

inline std::set<Vertex> doBisection(const Graph &G) {
    auto bisection = std::set<Vertex>();
    fillBisection(G, bisection);
    bisection = iterateBisection(G, bisection);
    return bisection;
}
#endif