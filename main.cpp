#include <iostream>
#include <set>
#include "../ba-graph/include/impl/basic/include.hpp"
#include "../ba-graph/include/impl/basic/graph.h"
//#include <time.h>
//#include <stdlib.h>

//#include "../ba-graph/include/graphs/stored_graphs.hpp"
//#include "../ba-graph/include/invariants.hpp"
//#include "../ba-graph/include/io.hpp"


using namespace ba_graph;

inline auto findCutSize(

        const std::set<Vertex> &partition,
        const Graph &G

) {
    int cutSize = 0;
    for (auto &rot : G) {
        if (partition.count(rot.v()) > 0) {
            auto it = rot.begin();
            while (it != rot.end()) {
                if (partition.count(it->v2()) == 0)
                    cutSize++;
                it.operator++();
            }
        }
    }
    return cutSize/2;
}

inline auto doBisection(

        const Graph &G

) {
    auto bisection = std::set<Vertex>();
    // double upperBound = G.order() / 6;
    int cutSize = 0,
        half = G.order() % 2 == 0 ? G.order() / 2 : (G.order() + 1) / 2,
        inserted = 0;
    for (auto &rot : G) {
        if (inserted < half) {
            bisection.insert(rot.v());
            inserted++;
        } else break;
    }
    cutSize = findCutSize(bisection, G);
    std::cout<< "Process started with cut size: " << cutSize << std::endl;
    Vertex removed, added;
    auto excludedAdd = std::set<Vertex>();
    auto excludedRemove = std::set<Vertex>();
    //while (cutSize > upperBound) {
    while (true) {
        int minCutSize = cutSize,
            minCutSizeDiff = 0;
        auto minBisection = std::set<Vertex>();
        bool addedSomething = false;
        for (auto &rot : G) {
            if (bisection.count(rot.v()) == 0 && excludedAdd.count(rot.v()) == 0) {
                auto bisection_temp = bisection;
                bisection_temp.insert(rot.v());
                int cutSize_temp = findCutSize(bisection_temp, G);
                int diff = cutSize - cutSize_temp;
                if (cutSize_temp <= minCutSize && diff >= 0){
                    addedSomething = true;
                    added = rot.v();
                    minBisection = bisection_temp;
                    minCutSize = cutSize_temp;
                    minCutSizeDiff = diff;
                }
            }
        }
        if (!addedSomething){
            std::cout << "Did not find any suitable Vertex to add. Cut size is: " << cutSize << std::endl;
            break;
        }
        if (minCutSize < cutSize) {
            std::cout << "Vertex was added that reduced cut size to: " << minCutSize << std::endl;
        } else {
            std::cout << "Vertex was added. Cut size is still: " << minCutSize << std::endl;
        }
        auto newCutSize = cutSize;
        std::set<Vertex> newBisection;
        bool removedSomething = false;
        for (auto &rot : G) {
            if (bisection.count(rot.v()) > 0 && excludedRemove.count(rot.v()) == 0) {
                auto bisection_temp = minBisection;
                bisection_temp.erase(rot.v());
                int cutSize_temp = findCutSize(bisection_temp, G);
                if (cutSize_temp <= minCutSize + minCutSizeDiff && cutSize_temp <= newCutSize) {
                    removedSomething = true;
                    removed = rot.v();
                    newBisection = bisection_temp;
                    newCutSize = cutSize_temp;
                }
            }
        }
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

/*
struct sVertex{
    Vertex v;
    int degree;
};

bool operator ==(sVertex sv1, sVertex sv2){
    return sv1.degree == sv2.degree && sv1.v == sv2.v;
}
*/
int main() {
    /*
    Graph g = Graph();
    Vertex v1 = createV();
    Vertex v2 = createV();
    Vertex v3 = createV();
    Vertex v4 = createV();
    Vertex v5 = createV();
    Vertex v6 = createV();


    srand(time(nullptr));


    auto vertices = std::vector<sVertex>();
    for (int k = 0; k < 50; ++k) {
        sVertex sV;
        sV.v = createV();
        sV.degree = 3;
        addV(g, sV.v);
        vertices.push_back(sV);
    }
    bool end = false;
    auto edges = std::set<std::pair<Vertex, Vertex>>();
    do{
        int r1 = rand() % vertices.size();
        int r2 = rand() % vertices.size();
        if (r1 == r2) {
            std::cout<<"=="<<std::endl;
            continue;
        }
        auto pair1 = std::pair<Vertex, Vertex>();
        pair1.first = vertices.at(r1).v;
        pair1.second = vertices.at(r2).v;
        auto pair2 = std::pair<Vertex, Vertex>();
        pair2.first = vertices.at(r2).v;
        pair2.second = vertices.at(r1).v;
        if (edges.count(pair1) == 0 && edges.count(pair2) == 0) {
            addE(g, vertices.at(r1).v, vertices.at(r2).v);
            edges.insert(pair1);
            edges.insert(pair2);
        }
        else
            continue;
        vertices.at(r1).degree--;
        vertices.at(r2).degree--;
        if (vertices.at(r1).degree == 0){
            vertices.at(r1) = vertices.back();
            vertices.pop_back();
        }
        if (vertices.at(r2).degree == 0){
            vertices.at(r2) = vertices.back();
            vertices.pop_back();
        }
        if (vertices.empty())
            end = true;
        std::cout<<vertices.size()<<" vertices size"<<std::endl;
    } while (!end);
*/
    /*
    addV(g, v1);
    addV(g, v2);

    addV(g, v3);
    addV(g, v4);

    addV(g, v5);
    addV(g, v6);

    addE(g, v1, v2);
    addE(g, v2, v3);
    addE(g, v2, v5);
    addE(g, v1, v4);

    addE(g, v4, v5);
    addE(g, v6, v5);
    addE(g, v3, v6);

    std::set<Vertex> s = std::set<Vertex>();
    s.insert(v1);
    s.insert(v4);
    s.insert(v5);*/

    /*Configuration cfg;
    cfg.loadFromString(R"({"storage": {"dir": "../../resources/graphs"}})");

    auto sg1 = StoredGraphs::create<SnarkStorageDataG4C4>(cfg);
    Graph g(sg1->get_graph(10, 0));

*/  int n,
        m;
    Graph g = Graph();
    std::cin >> n;

    for (int j = 0; j < n; ++j) {
        Vertex v = createV();
        addV(g, v);
    }
    for (int k = 0; k < n; ++k) {
        for (int j = 0; j < 3; ++j) {
            std::cin >> m;
            addE(g, g[k].v(), g[m].v());
        }
    }

    std::cout << g.size() << " " << g.order() << std::endl;

    std::set<Vertex> bisection = doBisection(g);

    std::cout << "Final cut size: " << findCutSize(bisection, g) << '\n';

    std::cout << "Number of vertices in bisection is " << bisection.size() << ", while number of vertices in graph is " << g.order() << std::endl;

    return 0;
}