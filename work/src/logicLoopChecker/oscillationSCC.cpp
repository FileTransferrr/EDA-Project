#include "oscillationSCC.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <stack>

bool mergeConditions(const std::vector<Gate>& gates, std::vector<condition>& conditions, const std::vector<condition>& conditions1, const std::vector<condition>& conditions2) {
    conditions = conditions1;
    for (int i = 0; i < conditions2.size(); ++i) {
        for (int j = 0; j < conditions.size(); ++j) {
            if (gates[conditions2[i].gateIndex].inputs[conditions2[i].portIndex - 1] == gates[conditions[j].gateIndex].inputs[conditions[j].portIndex - 1]) {
                if (conditions2[i].value != conditions[j].value) {
                    conditions.clear();
                    return false;
                }
            }
        }
        conditions.push_back(conditions2[i]);
    }
    return true;
}

bool judgeOscillationLoop(const std::vector<Gate>& gates, std::vector<int>& loop, std::vector<condition>& conditions){
    conditions.clear();
    std::vector<std::string> loopSignals;
    for (int i = 0; i < loop.size(); ++i){
        loopSignals.push_back(gates[loop[i]].output);
    }

    int inverter_sum = 0;
    for (int i = 0; i < loop.size(); ++i){
        Gate current_gate = gates[loop[i]];
        if (current_gate.type == "not1") {
            inverter_sum++;
            continue;
        }
            
        int outPort = 0; // The port that is not in the loop
        for (int j = 0; j < current_gate.inputs.size(); ++j) {
            if (std::find(loopSignals.begin(), loopSignals.end(), current_gate.inputs[j]) == loopSignals.end()) {
                outPort = j + 1;
                break;
            }
        }

        if (current_gate.type == "and2") {
            conditions.push_back({loop[i], outPort, true});
        }
        else if (current_gate.type == "or2") {
            conditions.push_back({loop[i], outPort, false});
        }
        else if (current_gate.type == "nand2") {
            conditions.push_back({loop[i], outPort, true});
            inverter_sum++;
        }
    }

    if (inverter_sum % 2 == 0) {
        conditions.clear();
        return false;
    }
    return true;
}


oscillationSCC::oscillationSCC(const std::vector<Gate>& gates, const std::vector<std::vector<int>>& graph, const sccInfo& sccInformation) : gates(gates), graph(graph), sccInformation(sccInformation) {
    loops = findSCCLoops(gates, graph, sccInformation.scc);
    loopConditions.resize(loops.size());
    oscillationLoops.assign(loops.size(), false);
    loopSignals.resize(loops.size());
    oscillationLoopCount = 0;
    for (int i = 0; i < loops.size(); ++i) {
        for (int j = 0; j < loops[i].size(); ++j) {
            loopSignals[i].push_back(gates[loops[i][j]].output);
        }

        if (judgeOscillationLoop(gates, loops[i], loopConditions[i])) {
            oscillationLoops[i] = true;
            oscillationLoopCount++;
        }
    }
    intConditions.resize(loops.size());
    extConditions.resize(loops.size());
    oscConditions.resize(loops.size());
}

std::vector<std::vector<condition>> oscillationSCC::judgeOscillation() {
    std::vector<std::vector<condition>> oscillationConditions;
    oscillationConditions.clear();
    itentifyConditions();

    if (oscillationLoopCount == 0) {
        return oscillationConditions;
    }
    if (oscillationLoopCount == loops.size()) {
        std::vector<condition> conditions;
        for (int i = 0; i < loops.size(); ++i) {
            for (int j = 0; j < extConditions[i].size(); ++j) {
                for (int k = 0; k < conditions.size(); ++k) {
                    if (extConditions[i][j].gateIndex == conditions[k].gateIndex && extConditions[i][j].portIndex == conditions[k].portIndex && extConditions[i][j].value == conditions[k].value) {
                        continue;
                    }
                }
                conditions.push_back(extConditions[i][j]);               
            }
        }
        oscillationConditions.push_back(conditions);
        return oscillationConditions;
    }
    buildRevSubGraph();
    for (int i = 0; i < loops.size(); ++i) {
        if (!oscillationLoops[i]) {
            continue;
        }
        bool oscillation = true;
        std::vector<condition> conditions;
        for (int j = 0; j < intConditions[i].size(); ++j) {
            int start = belongToPositiveLoop(intConditions[i][j]);
            if (start == -1) {
                continue;
            }
            std::vector<condition> subConditions;
            std::vector<condition> tempConditions = conditions;
            if (!travelRevGraph(intConditions[i][j].value, start, i, subConditions)) {
                oscillation = false;
                conditions.clear();
                break;
            }     

            if (!mergeConditions(gates, conditions, tempConditions, subConditions)) {
                oscillation = false;
                conditions.clear();
                break;
            }
        }
        if (oscillation) {
            std::vector<condition> finalConditions;
            mergeConditions(gates, finalConditions, conditions, extConditions[i]);
            oscConditions[i] = finalConditions;
            oscillationLoops[i] = true;
        }
        else {
            oscillationLoops[i] = false;
        }
    }

    oscillationLoopCount = 0;
    std::vector<std::vector<condition>> tempConditions;
    for (int i = 0; i < loops.size(); ++i) {
        if (oscillationLoops[i]) {
            oscillationLoopCount++;
            tempConditions.push_back(oscConditions[i]);
        }
    }

    if (oscillationLoopCount == 0) {
        return oscillationConditions;
    }
    else if (oscillationLoopCount == 1) {
        oscillationConditions.push_back(tempConditions[0]);
    }
    else {
        std::vector<condition> finalConditions;
        bool confilict = !mergeConditions(gates, finalConditions, tempConditions[0], tempConditions[1]);
        if (confilict) {
            int maxLoopSize = -1;
            int maxLoopIndex = -1;
            for (int i = 0; i < loops.size(); ++i) {
                if (oscillationLoops[i]) {
                    if (loops[i].size() > maxLoopSize) {
                        maxLoopSize = loops[i].size();
                        maxLoopIndex = i;
                    }       
                }
            }
            oscillationConditions.push_back(oscConditions[maxLoopIndex]);
        }
        else {
            oscillationConditions.push_back(finalConditions);
        }
    }

    std::unordered_set<std::string> assignedPins;
    for (int i = 0; i < oscillationConditions[0].size(); ++i) {
        assignedPins.insert(gates[oscillationConditions[0][i].gateIndex].inputs[oscillationConditions[0][i].portIndex - 1]);
    }

    for (int i = 0; i < sccInformation.scc.size(); ++i) {
        int node = sccInformation.scc[i];
        for (int j = 0; j < gates[node].inputs.size(); ++j) {
            if (std::find(sccInformation.loopSignals.begin(), sccInformation.loopSignals.end(), gates[node].inputs[j]) == sccInformation.loopSignals.end()) {
                if (assignedPins.find(gates[node].inputs[j]) == assignedPins.end()) {
                    if (gates[node].type == "and2") {
                        oscillationConditions[0].push_back({node, j + 1, true});
                    }
                    else if (gates[node].type == "or2") {
                        oscillationConditions[0].push_back({node, j + 1, false});
                    }
                    else if (gates[node].type == "nand2") {
                        oscillationConditions[0].push_back({node, j + 1, true});
                    }
                }
                break;
            }
        }
    }

    return oscillationConditions;
}

std::unordered_set<std::string> oscillationSCC::minBreakers() {
    std::unordered_set<std::string> breakers;
    if (oscillationLoopCount == 1) {
        for (int i = 0; i < loops.size(); ++i) {
            if (oscillationLoops[i]) {
                breakers.insert(gates[loops[i][0]].output);
                break;
            }
        }
    }
    else {
        for (int i = 0; i < loops.size(); ++i) {
            for (int j = 0; j < intConditions[i].size(); ++j) {
                breakers.insert(gates[intConditions[i][j].gateIndex].output);
            }
        }
    }
    return breakers;
}

void oscillationSCC::itentifyConditions() {
    for (int i = 0; i < loops.size(); ++i) {
        if (!oscillationLoops[i]) {
            continue;
        }
        for (int j = 0; j < loopConditions[i].size(); ++j) {
            if (std::find(sccInformation.loopSignals.begin(), sccInformation.loopSignals.end(), gates[loopConditions[i][j].gateIndex].inputs[loopConditions[i][j].portIndex - 1]) != sccInformation.loopSignals.end()) {
                std::vector<condition> newConditions;
                newConditions.push_back(loopConditions[i][j]);
                std::vector<condition> tempConditions = intConditions[i];
                if (!mergeConditions(gates, intConditions[i], tempConditions, newConditions)) {
                    intConditions[i].clear();
                    oscillationLoops[i] = false;
                    oscillationLoopCount--;
                    break;
                }
            }
            else {
                std::vector<condition> newConditions;
                newConditions.push_back(loopConditions[i][j]);
                std::vector<condition> tempConditions = extConditions[i];
                if (!mergeConditions(gates, extConditions[i], tempConditions, newConditions)) {
                    extConditions[i].clear();
                    oscillationLoops[i] = false;
                    oscillationLoopCount--;
                    break;
                }
            }
        }
    }
}

void oscillationSCC::buildRevSubGraph() {
    revGraph.resize(graph.size());
    std::unordered_set<int> subNodes;
    
    for (int i = 0; i < loops.size(); ++i) {
        if (oscillationLoops[i]) {
            continue;
        }
        for (int j = 0; j < loops[i].size(); ++j) {
            subNodes.insert(loops[i][j]);
        }
    }

    for (int i = 0; i < graph.size(); ++i) {
        if (subNodes.find(i) == subNodes.end()) {
            continue;
        }
        for (int j = 0; j < graph[i].size(); ++j) {
            if (subNodes.find(graph[i][j]) != subNodes.end()) {
                revGraph[graph[i][j]].push_back(i);
            }
        }
    }
}

int oscillationSCC::belongToPositiveLoop(const condition& c) {
    for (int i = 0; i < loops.size(); ++i) {
        if (oscillationLoops[i]) {
            continue;
        }
        for (int j = 0; j < loops[i].size(); ++j) {
            if (gates[loops[i][j]].output == gates[c.gateIndex].inputs[c.portIndex - 1]) {
                return loops[i][j];
            }
        }
    }
    return -1;
}

void oscillationSCC::backwardReasoning(const bool& outValue, const std::string& type, int& inValue1, int& inValue2) {
    if (type == "and2") {
        if (outValue) {
            inValue1 = 1;
            inValue2 = 1;
        }
        else {
            inValue1 = 0;
            inValue2 = 2;
        }
    }
    else if (type == "or2") {
        if (outValue) {
            inValue1 = 1;
            inValue2 = 2;
        }
        else {
            inValue1 = 0;
            inValue2 = 0;
        }
    }
    else if (type == "nand2") {
        if (outValue) {
            inValue1 = 0;
            inValue2 = 2;
        }
        else {
            inValue1 = 1;
            inValue2 = 1;
        }
    }
}

bool oscillationSCC::travelRevGraph(const bool& startValue, const int& start, const int& loopIndex, std::vector<condition>& conditions) {
    conditions.clear();
    std::unordered_map<std::string, bool> values;
    values[gates[start].output] = startValue;

    std::stack<int> s;
    s.push(start);
    std::unordered_set<int> visited;
    while (!s.empty()) {
        int current = s.top();
        s.pop();
        visited.insert(current);

        if (gates[current].type == "not1") {
            visited.insert(revGraph[current][0]);
            s.push(revGraph[current][0]);
            values[gates[current].inputs[0]] = !values[gates[current].output];
            continue;
        }

        int inValue1, inValue2;
        backwardReasoning(values[gates[current].output], gates[current].type, inValue1, inValue2);

        int count = gates[current].inputs.size() - revGraph[current].size();
        if (count == 1) { // one input is not in the loop
            int extPort;
            int intPort;
            std::string nextOutput = gates[revGraph[current][0]].output; 
            for (int i = 0; i < gates[current].inputs.size(); ++i) {
                if (gates[current].inputs[i] == nextOutput) {
                    intPort = i;
                }
                else {
                    extPort = i;
                }
            }

            if (std::find(sccInformation.loopSignals.begin(), sccInformation.loopSignals.end(), gates[current].inputs[extPort]) == sccInformation.loopSignals.end()) {
                // external pin
                conditions.push_back({current, extPort + 1, inValue1 == 1 ? true : false});
                if (inValue2 != 2) {
                    values[gates[current].inputs[intPort]] = inValue2 == 1 ? true : false;
                    s.push(revGraph[current][0]);
                }
            }
            else {
                if (std::find(loopSignals[loopIndex].begin(), loopSignals[loopIndex].end(), gates[current].inputs[extPort]) != loopSignals[loopIndex].end()) {
                    if (inValue2 == 2) {
                        values[gates[current].inputs[intPort]] = inValue1 == 1 ? true : false;
                        s.push(revGraph[current][0]);
                    }
                    else {
                        conditions.clear();
                        return false;
                    }
                }
                else {
                    values[gates[current].inputs[intPort]] = inValue1 == 1 ? true : false;
                    s.push(revGraph[current][0]);
                }
            }
        }
        else { // count == 0
            values[gates[current].inputs[0]] = inValue1 == 1 ? true : false;
            s.push(revGraph[current][0]);
            if (inValue2 != 2) {
                values[gates[current].inputs[1]] = inValue2 == 1 ? true : false;
                s.push(revGraph[current][1]);
            }
        }
    }
    return true;
}