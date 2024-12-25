#ifndef TARJAN_HPP
#define TARJAN_HPP

#include <vector>
#include <stack>

class Tarjan {
public:
    Tarjan(const std::vector<std::vector<int>>& graph);
    std::vector<std::vector<int>> findStronglyConnectedComponents();

private:
    const std::vector<std::vector<int>>& graph;
    std::vector<int> low, indices;
    std::vector<bool> onStack;
    std::stack<int> s;
    int index;
    std::vector<std::vector<int>> sccs;

    void strongConnect(int v);
};

#endif // TARJAN_HPP