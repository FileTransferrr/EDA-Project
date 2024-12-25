#ifndef LOOPFINDER_HPP
#define LOOPFINDER_HPP

#include <vector>
#include "gate.hpp"

class loopFinder {
public:
    loopFinder(const std::vector<std::vector<int>>& graph);
    std::vector<std::vector<int>> findLoops();

private:
    const std::vector<std::vector<int>>& graph;
    int n;
    std::vector<std::vector<int>> loops;

    void dfs(int v, std::vector<bool>& inStack, std::vector<int>& stack);
    void printLoop(int start, const std::vector<int>& stack);
};

std::vector<std::vector<int>> findSCCLoops(const std::vector<Gate>& gates, const std::vector<std::vector<int>>& graph, const std::vector<int>& scc);

#endif // LOOPFINDER_HPP