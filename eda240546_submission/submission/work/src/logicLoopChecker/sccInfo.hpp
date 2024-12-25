#ifndef SCCINFO_HPP
#define SCCINFO_HPP

#include "gate.hpp"
#include <vector>
#include <string>

struct sccInfo {
    std::vector<int> scc;
    std::vector<std::string> loopSignals;
    std::vector<std::string> loopGates;
};

std::vector<sccInfo> saveSCCs(const std::vector<Gate>& gates, const std::vector<std::vector<int>>& sccs);

void printAllSCCs(const std::vector<sccInfo>& sccInfos);

#endif // SCCINFO_HPP