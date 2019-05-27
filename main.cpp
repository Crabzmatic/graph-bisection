#include <ctime>
#include <iterativeBisection.hpp>
#include <growingBisection.hpp>
#include <combinedBisection.hpp>

void printBisectionStats(const Graph &g, clock_t t, const BisectionResult &result, const std::string &bisectionMethod);

int main() {
    int n, m;
    Graph g(createG());
    std::cin >> n;
    for (int j = 0; j < n; ++j) {
        Vertex v(createV());
        addV(g, v);
    }
    for (int k = 0; k < n; ++k) {
        for (int j = 0; j < 3; ++j) {
            std::cin >> m;
            if (k < m)
                addE(g, g[k].v(), g[m].v());
        }
    }
    std::cout << "Graph edge count: " << g.size() << " vertex count: " << g.order() << std::endl;

    // BISECTION 1
    clock_t t1 = clock();
    BisectionResult result = doIterativeBisection(g);
    t1 = clock() - t1;
    printBisectionStats(g, t1, result, "Single vertex exchange iterative bisection");

    // BISECTION 2
    clock_t t2 = clock();
    result = doGrowingBisection(g, false);
    t2 = clock() - t2;
    printBisectionStats(g, t2, result, "Parallel growing bisection");

    // BISECTION 3
    clock_t t3 = clock();
    result = doCombinedBisection(g);
    t3 = clock() - t3;
    printBisectionStats(g, t3, result, "Iterative with growing starting bisection");
    return 0;
}

void printBisectionStats(const Graph &g, clock_t t, const BisectionResult &result, const std::string &bisectionMethod) {
    std::cout << std::endl;
    std::cout << bisectionMethod << ":" << std::endl;
    std::cout << "--Final cut size is " << result.getCutSize() << std::endl;
    std::cout << "--Number of vertices in bisection is " << result.getBisection().size()
              << ", while number of vertices in graph is " << g.order() << std::endl;
    std::cout << "--Elapsed time: " << (float) t / CLOCKS_PER_SEC << " seconds" << std::endl;
}