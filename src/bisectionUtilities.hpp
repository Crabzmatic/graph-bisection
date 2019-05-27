#ifndef BISECTION_UTILITIES_HPP
#define BISECTION_UTILITIES_HPP
#include <thread>
#include <future>
#include <random>
#include "../../ba-graph/include/impl/basic/include.hpp"
#include "../../ba-graph/include/impl/basic/graph.h"
inline unsigned long long graphHalf(const Graph &G){
    return G.order() % 2 == 0 ? G.order() / 2 : (G.order() + 1) / 2;
}
inline unsigned int hardwareConcurrency(){
    return std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 1;
}
template <typename Func, typename Result>
inline void runAsParallel(
        const Graph &G,
        Func function,
        std::vector<std::future<Result>> &futuresOut,
        unsigned int threadNum = hardwareConcurrency()
) {
    auto threads = std::vector<std::thread>();
    auto promises = std::vector<std::promise<Result>>();
    std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dist(0, G.order() - 1);
    for (unsigned int i = 0; i < threadNum; ++i) {
        promises.emplace_back();
        futuresOut.push_back(promises[i].get_future());
        threads.emplace_back(function, std::move(promises[i]), std::ref(G), std::ref(G[dist(gen)].v()));
    }
    for (unsigned int i = 0; i < threadNum; ++i) {
        threads[i].join();
    }
}
#endif
