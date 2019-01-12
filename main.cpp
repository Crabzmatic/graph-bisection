#include "bisection.hpp"

int main() {
    int n, m;
    Graph g = Graph();
    std::cin >> n;

    for (int j = 0; j < n; ++j) {
        Vertex v = createV();
        addV(g, v);
    }

    for (int k = 0; k < n; ++k) {
        for (int j = 0; j < 3; ++j) {
            std::cin >> m;
            if (k < m)
                addE(g, g[k].v(), g[m].v());
        }
    }

    std::cout << g.size() << " " << g.order() << std::endl;

    std::set<Vertex> bisection = doBisection(g);

    std::cout << "Final cut size: " << findCutSize(g, bisection) << '\n';

    std::cout << "Number of vertices in bisection is " << bisection.size() << ", while number of vertices in graph is " << g.order() << std::endl;

    return 0;
}