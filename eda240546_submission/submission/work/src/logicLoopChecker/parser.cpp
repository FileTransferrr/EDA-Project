#include "parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void parseModInfo(const std::string& filename, std::vector<Gate>& gates) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line.find("module") != std::string::npos) {
            continue;
        }

        size_t colonPos = line.find(":");
        if (colonPos != std::string::npos) {
            line = line.substr(colonPos + 1);
        }

        std::istringstream lineStream(line);
        std::string gateType, gateName, output;
        std::vector<std::string> inputs;

        lineStream >> gateType >> gateName >> output;

        std::string input;
        while (lineStream >> input) {
            inputs.push_back(input);
        }

        Gate gate;
        gate.type = gateType;
        gate.name = gateName;
        gate.inputs = inputs;
        gate.output = output;

        gates.push_back(gate);
    }
}

void buildGraph(const std::vector<Gate>& gates, std::vector<std::vector<int>>& graph) {
    std::unordered_map<std::string, int> gateMap;
    for (int i = 0; i < gates.size(); ++i) {
        const Gate& gate = gates[i];
        gateMap[gate.output] = i;
    }

    graph.resize(gates.size());
    for (int i = 0; i < gates.size(); ++i) {
        const Gate& gate = gates[i];
        for (const std::string& input : gate.inputs) {
            if (gateMap.find(input) != gateMap.end()) {
                graph[gateMap[input]].push_back(i);
            }
        }
    }
}