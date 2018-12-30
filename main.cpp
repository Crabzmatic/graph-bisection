#include <iostream>
#include <set>
#include "../ba-graph/include/impl/basic/include.hpp"
#include "../ba-graph/include/impl/basic/graph.h"

using namespace ba_graph;

inline auto findCutSize(const std::set<Vertex> &bsc, const Graph &G){
    int cutSize = 0;
    for (auto &it : G) {
        if (bsc.count(it.v()) == 0) continue;
        else {
            for (auto &it2 : it) {
                Vertex neighbour = it2.e().other_v(it.v());
                if (bsc.count(neighbour) == 0)
                    cutSize++;
            }
        }
    }
    return cutSize;
}
inline auto doBisection(const Graph &G){
    auto bsc = std::set<Vertex>();

    int cutSize = 0,
        upperBound = (1/6) * G.order(),
        half = G.order() % 2 == 0 ? G.order() / 2 : (G.order() + 1) / 2;

    for (int i = 0; i < half; i++) {
        bsc.insert(G[i].v());
    }

    cutSize = findCutSize(bsc, G);

    while(cutSize > upperBound){
        //moveS0toV1(&bsc, &G);
        //moveS1toV0(&bsc, &G);
        cutSize = findCutSize(bsc, G);
    }

    return bsc;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    Graph g = Graph();
    Vertex v1 = Vertex();
    Vertex v2 = Vertex();
    Vertex v3 = Vertex();
    Vertex v4 = Vertex();
    Vertex v5 = Vertex();
    Vertex v6 = Vertex();

    addV(g, v1);
    addV(g, v2);

    addV(g, v3);
    addV(g, v4);

    addV(g, v5);
    addV(g, v6);

    addE(g, v1, v2);
    addE(g, v1, v3);
    addE(g, v1, v4);
    addE(g, v1, v5);

    addE(g, v5, v6);

    std::set<Vertex> s = std::set<Vertex>();
    s.insert(v1);
    std::cout<<findCutSize(s, g)<<'\n';

/*
    for (auto i : g.find(v1)->neighbours()){
        std::cout<<i.n<<'\n';
    }

    for (auto i : g.find(v5)->neighbours()){
        std::cout<<i.n<<'\n';
    }

    if (g.contains(v1))
        std::cout<<"Contains!"<<std::endl;
        */
    return 0;
}