#ifndef PRINTRESULT_HPP
#define PRINTRESULT_HPP

#include "gate.hpp"
#include "sccInfo.hpp"
#include "oscillationSCC.hpp"
#include <vector>
#include <string>

struct sccResult {
    bool osc;
    std::vector<std::vector<std::string>> conditions;
    std::vector<std::string> breakers;
};

class printResult {
public:
    printResult(const std::vector<Gate>& gates, const std::vector<std::vector<int>>& graph, const std::vector<sccInfo>& sccInfos);

    void calculateAllSCCs();

private:
    std::vector<Gate> gates;
    std::vector<std::vector<int>> graph;
    std::vector<sccInfo> sccInfos;
    std::vector<sccResult> sccResults;

    std::vector<std::vector<int>> buildSubGraph(const std::vector<int>& scc);

    void printAllSCCs();

    void calculateSCCs(int start, int end);
};

#endif // PRINTRESULT_HPP