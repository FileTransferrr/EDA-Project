#include "loopFinder.hpp"
#include <algorithm>

loopFinder::loopFinder(const std::vector<std::vector<int>>& graph) : graph(graph), n(graph.size()) {
    loops.clear();
}

std::vector<std::vector<int>> loopFinder::findLoops() {
    std::vector<bool> inStack(n, false);
    std::vector<int> stack;
    for (int v = 0; v < n; ++v) {
        if (graph[v].size() > 0) {
            dfs(v, inStack, stack);
            break;
        }
    }
    return loops;
}

void loopFinder::dfs(int v, std::vector<bool>& inStack, std::vector<int>& stack) {
    inStack[v] = true;
    stack.push_back(v);

    for (int w : graph[v]) {
        if (inStack[w]) {
            printLoop(w, stack);
        } else {
            dfs(w, inStack, stack);
        }
    }

    inStack[v] = false;
    stack.pop_back();
}

void loopFinder::printLoop(int start, const std::vector<int>& stack) {
    std::vector<int> loop;
    bool print = false;
    for (int v : stack) {
        if (print || v == start) {
            loop.push_back(v);
            print = true;
        }
    }
    loops.push_back(loop);
}

std::vector<std::vector<int>> findSCCLoops(const std::vector<Gate>& gates, const std::vector<std::vector<int>>& graph, const std::vector<int>& scc) {
    std::vector<std::vector<int>> sccGraph(graph.size());
    for (int i = 0; i < scc.size(); ++i) {
        for (int j : graph[scc[i]]) {
            if (std::find(scc.begin(), scc.end(), j) != scc.end()) {
                sccGraph[scc[i]].push_back(j);
            }
        }
    }

    loopFinder loopFinder(sccGraph);
    std::vector<std::vector<int>> loops = loopFinder.findLoops();
    return loops;
}