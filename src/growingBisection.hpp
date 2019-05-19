#include <utility>

#ifndef GROWING_BISECTION_HPP
#define GROWING_BISECTION_HPP
#ifdef DEBUG
#define D if(true)
#else
#define D if(false)
#endif
#include <set>
#include <thread>
#include <future>
#include <random>
#include "../../ba-graph/include/impl/basic/include.hpp"
#include "../../ba-graph/include/impl/basic/graph.h"
#include "bisectionResult.hpp"
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

void growBisection(std::promise<ParallelBisectionResult> && promise, const Graph &G, std::set<Vertex> &&bisection) {
    auto toAdd = std::set<Vertex>();
    auto unvisited = std::vector<Vertex>();
    for (auto &it : G){
        unvisited.push_back(it.v());
    }
    Vertex seed = *bisection.begin();
    for (unsigned int i = 0; i < unvisited.size(); ++i){
        if (unvisited[i] == seed){
            unvisited.erase(unvisited.begin() + i);
        }
    }
    RotationIterator rot = G.find(seed);
    for (auto & it : *rot){
        if (it.v1() == seed){
            toAdd.insert(it.v2());
        }
    }
    unsigned long long limit = G.order()/2;
    int cutSize = findCutSize(G, bisection);
    std::random_device rnd;
    std::mt19937 gen(rnd());
    while(bisection.size() < limit){
        if (toAdd.empty()){
            std::uniform_int_distribution<> dist(0, unvisited.size() - 1);
            toAdd.insert(unvisited[dist(gen)]);
        }
        int minimalGain = INT32_MAX;
        Vertex vertexToAdd;
        for(const auto& v : toAdd){
            auto bsc_temp = bisection;
            bsc_temp.insert(v);
            int gain = findCutSize(G, bsc_temp) - findCutSize(G, bisection);
            if (minimalGain > gain) {
                minimalGain = gain;
                vertexToAdd = v;
            }
        }
        toAdd.erase(vertexToAdd);
        bisection.insert(vertexToAdd);
        for (unsigned int i = 0; i < unvisited.size(); ++i){
            if (unvisited[i] == vertexToAdd){
                unvisited.erase(unvisited.begin() + i);
            }
        }
        cutSize += minimalGain;
        auto r = G.find(vertexToAdd);
        auto adjacent = std::set<Vertex>();
        for(auto & it : *r){
            if (it.v1() == vertexToAdd){
                adjacent.insert(it.v2());
            }
        }
        if (!adjacent.empty()){
            for (const auto& v : adjacent){
                if (toAdd.count(v) == 0 && bisection.count(v) == 0){
                    toAdd.insert(v);
                }
            }
        }
    }
    auto result = ParallelBisectionResult(bisection, cutSize);
    return promise.set_value(result);
}

inline BisectionResult doGrowingBisection(const Graph &G) {
    unsigned threadNum = std::thread::hardware_concurrency();
    if (threadNum == 0){
        threadNum = 1;
    }
    if (G.order() < 300){
        threadNum = G.order();
    }
    auto threads = std::vector<std::thread>();
    auto promises = std::vector<std::promise<ParallelBisectionResult>>();
    auto futures = std::vector<std::future<ParallelBisectionResult>>();
    std::random_device r;
    std::mt19937 gen(r());
    std::uniform_int_distribution<> dist(0, G.order() - 1);
    for (unsigned int i = 0; i < threadNum; ++i) {
        auto bisection = std::set<Vertex>();
        int index = G.order() < 300 ? i : dist(gen);
        bisection.insert(G[index].v());
        promises.emplace_back();
        futures.push_back(promises[i].get_future());
        threads.emplace_back(growBisection, std::move(promises[i]), std::ref(G), std::move(bisection));
    }
    for (unsigned int i = 0; i < threadNum; ++i) {
        threads[i].join();
    }
    BisectionResult *minimal = new ParallelBisectionResult(INT32_MAX);
    for (unsigned int i = 0; i < threadNum; ++i) {
        auto res = futures[i].get();
        //std::cout<<"INSIDE THREAD #"<<i<<" cut size is "<<res.getCutSize()<<std::endl;
        if (minimal->getCutSize() > res.getCutSize()){
            minimal->setBisection(res.getBisection());
            minimal->setCutSize(res.getCutSize());
        }
    }
    return *minimal;
}
#endif