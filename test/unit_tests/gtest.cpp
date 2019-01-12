#include <gtest/gtest.h>
#include "bisection.hpp"

TEST(testInput, degree_cubic)
{
    Graph g = Graph();

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

TEST(testBisection, bisectionOnSquareWithDiagonals)
{
    Graph g = Graph();

    for (int j = 0; j < 4; ++j) {
        addV(g, createV());
    }

    for (int k = 0; k < 4; ++k) {
        for (int j = 0; j < 4; ++j) {
            if (k < j)
                addE(g, g[k].v(), g[j].v());
        }
    }

    EXPECT_EQ(findCutSize(g,doBisection(g)), 4);
}

TEST(testBisection, CutSizeOneVertexSquareWithDiagonals)
{
    Graph g = Graph();

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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}