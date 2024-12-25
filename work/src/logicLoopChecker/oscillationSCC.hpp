#ifndef OSCILLATIONSCC_HPP
#define OSCILLATIONSCC_HPP

#include "gate.hpp"
#include "sccInfo.hpp"
#include "loopFinder.hpp"
#include <vector>
#include <unordered_set>

struct condition {
    int gateIndex;
    int portIndex;
    bool value;
};

bool mergeConditions(const std::vector<Gate>& gates, std::vector<condition>& conditions, const std::vector<condition>& conditions1, const std::vector<condition>& conditions2);

bool judgeOscillationLoop(const std::vector<Gate>& gates, std::vector<int>& loop, std::vector<condition>& conditions);

class oscillationSCC {
public:
    oscillationSCC(const std::vector<Gate>& gates, const std::vector<std::vector<int>>& graph, const sccInfo& sccInformation);

    int oscillationLoopCount;
    std::vector<bool> oscillationLoops;
    std::vector<std::vector<int>> loops;

    std::vector<std::vector<condition>> judgeOscillation();

    std::unordered_set<std::string> minBreakers();

private:
    std::vector<Gate> gates;
    std::vector<std::vector<int>> graph;
    sccInfo sccInformation;
    std::vector<std::vector<std::string>> loopSignals;
    std::vector<std::vector<condition>> loopConditions; // Consider the loops respectively
    std::vector<std::vector<condition>> oscConditions; // final result
    std::vector<std::vector<condition>> intConditions;
    std::vector<std::vector<condition>> extConditions;
    std::vector<std::vector<int>> revGraph;

    void itentifyConditions();

    void buildRevSubGraph();

    int belongToPositiveLoop(const condition& c);

    void backwardReasoning(const bool& outValue, const std::string& type, int& inValue1, int& inValue2);

    bool travelRevGraph(const bool& startValue, const int& start, const int& loopIndex, std::vector<condition>& conditions);
};

#endif // OSCILLATIONSCC_HPP