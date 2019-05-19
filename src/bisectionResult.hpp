#include <utility>

#ifndef GRAPH_BISECTION_BISECTIONRESULT_HPP
#define GRAPH_BISECTION_BISECTIONRESULT_HPP

using namespace ba_graph;
class BisectionResult{
protected:
    std::set<Vertex> bisection;
    int cutSize;
public:
    virtual int getCutSize() const {return cutSize;}
    virtual void setCutSize(int _cutSize){this->cutSize = _cutSize;}
    virtual std::set<Vertex> getBisection() const {return bisection;}
    virtual void setBisection(const std::set<Vertex> &_bisection){this->bisection = _bisection;}
};

#endif
