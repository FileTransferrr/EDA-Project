#include "tarjan.hpp"

Tarjan::Tarjan(const std::vector<std::vector<int>>& graph) : graph(graph), index(0) {
    int n = graph.size();
    low.resize(n, -1);
    indices.resize(n, -1);
    onStack.resize(n, false);
    sccs.clear();
}

std::vector<std::vector<int>> Tarjan::findStronglyConnectedComponents() {
    for (int v = 0; v < graph.size(); ++v) {
        if (indices[v] == -1) {
            strongConnect(v);
        }
    }
    return sccs;
}

void Tarjan::strongConnect(int v) {
    indices[v] = index;
    low[v] = index;
    index++;
    s.push(v);
    onStack[v] = true;

    for (int w : graph[v]) {
        if (indices[w] == -1) {
            strongConnect(w);
            low[v] = std::min(low[v], low[w]);
        } else if (onStack[w]) {
            low[v] = std::min(low[v], indices[w]);
        }
    }

    if (low[v] == indices[v]) {
        std::vector<int> scc;
        while (true) {
            int w = s.top();
            s.pop();
            onStack[w] = false;
            scc.push_back(w);
            if (w == v) break;
        }
        sccs.push_back(scc);
    }
}