#include <gtest/gtest.h>
#include <iterativeBisection.hpp>
#include <bisectionResult.hpp>
#include <growingBisection.hpp>
#include <future>

TEST(testInput, degree_cubic)
{
    Graph g(createG());
    for (int j = 0; j < 4; ++j) {
        addV(g, createV());
    }
    for (int k = 0; k < 4; ++k) {
        for (int j = 0; j < 4; ++j) {
            if (k < j)
                addE(g, g[k].v(), g[j].v());
        }
    }
    bool degreeCubic = true;
    for (auto &rot : g) {
        if (rot.degree() != 3)
            degreeCubic = false;
    }
    EXPECT_EQ(degreeCubic, true);
}
TEST(testIterativeBisection, bisectionOnSquareWithDiagonals)
{
    Graph g(createG());
    for (int j = 0; j < 4; ++j) {
        addV(g, createV());
    }
    for (int k = 0; k < 4; ++k) {
        for (int j = 0; j < 4; ++j) {
            if (k < j)
                addE(g, g[k].v(), g[j].v());
        }
    }
    auto bsc = doIterativeBisection(g);
    EXPECT_EQ(findCutSize(g, bsc.getBisection()), 4);
}
TEST(findCutSize, CutSizeOneVertexSquareWithDiagonals)
{
    Graph g(createG());
    for (int j = 0; j < 4; ++j) {
        addV(g, createV());
    }
    for (int k = 0; k < 4; ++k) {
        for (int j = 0; j < 4; ++j) {
            if (k < j)
                addE(g, g[k].v(), g[j].v());
        }
    }
    auto bisection = std::set<Vertex>();
    bisection.insert(g[0].v());
    EXPECT_EQ(findCutSize(g, bisection), 3);
}
TEST(growBisection, limit)
{
    Graph g(createG());
    for (int j = 0; j < 4; ++j) {
        addV(g, createV());
    }
    EXPECT_EQ(growBisection(g, g[0].v()).getBisection().size(), 2);
}
TEST(growBisection, simpleCutSize)
{
    Graph g(createG());
    for (int j = 0; j < 4; ++j) {
        addV(g, createV());
    }
    EXPECT_EQ(growBisection(g, g[0].v()).getCutSize(), 0);
}
TEST(growBisection, bestVertexTest)
{
    Graph g(createG());
    for (int j = 0; j < 6; ++j) {
        addV(g, createV());
    }
    addE(g, g[0].v(), g[2].v());
    addE(g, g[0].v(), g[3].v());
    addE(g, g[2].v(), g[5].v());
    addE(g, g[2].v(), g[3].v());
    addE(g, g[3].v(), g[1].v());
    addE(g, g[3].v(), g[4].v());

    /*
        0 o        o 1
          | \____  |
          |      \ |
        2 o ------ o 3
          |        |
        5 o        o 4

     */
    EXPECT_EQ(growBisection(g, g[0].v()).getCutSize(), 2);
}
TEST(growBisection, insertNeighbors)
{
    Graph g(createG());
    for (int j = 0; j < 6; ++j) {
        addV(g, createV());
    }
    addE(g, g[0].v(), g[2].v());
    addE(g, g[0].v(), g[3].v());
    addE(g, g[2].v(), g[5].v());
    addE(g, g[2].v(), g[3].v());
    addE(g, g[3].v(), g[1].v());
    addE(g, g[3].v(), g[4].v());
    /*  0 o        o 1
          | \____  |
          |      \ |
        2 o ------ o 3
          |        |
        5 o        o 4  */
    auto neighbors = std::set<Vertex>();
    insertNeighbors(neighbors, g.find(g[3].v()));
    EXPECT_EQ(neighbors.size(), 4);
}
TEST(growBisection, removeVertex)
{
    Graph g(createG());
    auto vertices = std::vector<Vertex>();
    for (int j = 0; j < 4; ++j) {
        Vertex v(createV());
        addV(g, v);
        vertices.push_back(v);
    }
    removeVertex(vertices, g[0].v());
    EXPECT_EQ(vertices.size(), 3);
}
TEST(growBisection, addBestVertex)
{
    Graph g(createG());
    for (int j = 0; j < 6; ++j) {
        addV(g, createV());
    }
    addE(g, g[0].v(), g[1].v());
    addE(g, g[0].v(), g[2].v());
    addE(g, g[0].v(), g[3].v());
    addE(g, g[2].v(), g[5].v());
    addE(g, g[2].v(), g[3].v());
    addE(g, g[3].v(), g[1].v());
    addE(g, g[3].v(), g[4].v());
    /*  0 o ------ o 1
          | \____  |
          |      \ |
        2 o ------ o 3
          |        |
        5 o        o 4  */
    auto bisection = std::set<Vertex>{g[3].v()};
    auto toAdd = std::set<Vertex>{g[0].v(), g[1].v(), g[2].v(), g[4].v()};
    auto unvisited = std::vector<Vertex>{g[5].v()};
    (void) addBestVertex(g, bisection, toAdd, unvisited, findCutSize(g, std::set<Vertex>{g[3].v()}));
    EXPECT_TRUE(bisection.count(g[4].v()) > 0);
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}