#include "sccInfo.hpp"
#include <algorithm>
#include <fstream>

std::vector<sccInfo> saveSCCs(const std::vector<Gate>& gates, const std::vector<std::vector<int>>& sccs) {
    std::vector<sccInfo> sccInfos;
    for (int i = 0; i < sccs.size(); ++i) {
        if (sccs[i].size() == 1) {
            continue;
        }
        sccInfo info;
        info.scc = sccs[i];
        std::vector<std::string> loopSignals;
        std::vector<std::string> loopGates;
        for (int j : sccs[i]) {
            loopSignals.push_back(gates[j].output);
        }
        for (int j : sccs[i]) {
            for (int k = 0; k < gates[j].inputs.size(); ++k) {
                std::string input = gates[j].inputs[k];
                if (std::find(loopSignals.begin(), loopSignals.end(), input) != loopSignals.end()) {
                    loopGates.push_back(gates[j].name + ".port" + std::to_string(k + 1));
                }
            }
        }
        std::sort(loopSignals.begin(), loopSignals.end());
        std::sort(loopGates.begin(), loopGates.end());
        info.loopSignals = loopSignals;
        info.loopGates = loopGates;
        sccInfos.push_back(info);
    }
    return sccInfos;
}
