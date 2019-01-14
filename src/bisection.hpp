#ifndef BISECTION_HPP
#define BISECTION_HPP

#include <set>
#include <iostream>
#include "../../ba-graph/include/impl/basic/include.hpp"
#include "../../ba-graph/include/impl/basic/graph.h"

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

struct Removing{
    Vertex removed;
    bool removedSomething;
    std::set<Vertex> v_removed;

    Removing(){
        v_removed = std::set<Vertex>();
        removedSomething = false;
    }
};

struct Adding{
    Vertex added;
    bool addedSomething;
    std::set<Vertex> v_added;

    Adding(){
        v_added = std::set<Vertex>();
        addedSomething = false;
    }
};

struct NewBsc{
    std::set<Vertex> newBsc;
    int newCutSize;

    NewBsc(){
        newBsc = std::set<Vertex>();
        newCutSize = 0;
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
        Adding &adding
) {
    MinBsc minBsc = MinBsc();
    minBsc.minCutSize = cutSize;
    adding.addedSomething = false;

    for (auto &rot : G) {

        if (bisection.count(rot.v()) == 0 && adding.v_added.count(rot.v()) == 0) {
            auto bisection_temp = bisection;
            bisection_temp.insert(rot.v());

            int cutSize_temp = findCutSize(G, bisection_temp);
            int diff = cutSize - cutSize_temp;

            if (cutSize_temp <= minBsc.minCutSize && diff >= 0) {
                adding.addedSomething = true;
                adding.added = rot.v();
                minBsc.minBsc = bisection_temp;
                minBsc.minCutSize = cutSize_temp;
                minBsc.minCutSizeDiff = diff;
            }
        }
    }
    return minBsc;
}

inline NewBsc removeVertexInBisection(
        const Graph &G,
        const std::set<Vertex> &bisection,
        int cutSize,
        Removing &removing,
        MinBsc &minBsc
) {
    NewBsc newBsc = NewBsc();
    newBsc.newCutSize = cutSize;
    removing.removedSomething = false;
    for (auto &rot : G) {

        if (bisection.count(rot.v()) > 0 && removing.v_removed.count(rot.v()) == 0) {
            auto bisection_temp = minBsc.minBsc;
            bisection_temp.erase(rot.v());

            int cutSize_temp = findCutSize(G, bisection_temp);

            if (cutSize_temp <= minBsc.minCutSize + minBsc.minCutSizeDiff && cutSize_temp <= newBsc.newCutSize) {
                removing.removedSomething = true;
                removing.removed = rot.v();
                newBsc.newBsc = bisection_temp;
                newBsc.newCutSize = cutSize_temp;
            }
        }
    }
    return newBsc;
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

inline std::set<Vertex> iterateBisection(const Graph &G, std::set<Vertex> &bisection) {

    int cutSize = findCutSize(G, bisection);
    //std::cout<< "Process started with cut size: " << cutSize << std::endl;

    Removing removing = Removing();
    Adding adding = Adding();

    while (true) {

        MinBsc minBsc = addVertexInBisection(G, bisection, cutSize, adding);

        if (!adding.addedSomething){
            //std::cout << "Did not find any suitable Vertex to add. Cut size is: " << cutSize << std::endl;
            break;
        }
        /*if (minBsc.minCutSize < cutSize) {
            std::cout << "Vertex was added that reduced cut size to: " << minBsc.minCutSize << std::endl;
        } else {
            std::cout << "Vertex was added. Cut size is still: " << minBsc.minCutSize << std::endl;
        }*/

        NewBsc newBisection = removeVertexInBisection(G, bisection, cutSize, removing, minBsc);

        if (!removing.removedSomething) {
            //std::cout << "Did not find any suitable Vertex to remove. Will not consider adding the same Vertex in next iteration." << std::endl;
            adding.v_added.insert(adding.added);
            continue;
        }
        if (adding.added == removing.removed){
            //std::cout << "Tried to remove the Vertex that was added. Will not consider removing this Vertex in next iteration." << std::endl;
            removing.v_removed.insert(removing.removed);
            continue;
        }
        if (newBisection.newCutSize == cutSize) {
            //std::cout << "Cut size did not change after this iteration. Will not consider adding the same Vertex in next iteration." << std::endl;
            adding.v_added.insert(adding.added);
            removing.v_removed.clear();
            continue;
        }
        cutSize = newBisection.newCutSize;
        bisection = newBisection.newBsc;
        adding.v_added.clear();
        removing.v_removed.clear();
        //std::cout << "Vertex removed. Successfully reduced cut size to: " << cutSize << std::endl;
    }
    return bisection;
}

inline std::set<Vertex> doBisection(const Graph &G) {
    auto bisection = std::set<Vertex>();
    fillBisection(G, bisection);
    return iterateBisection(G, bisection);
}
#endif