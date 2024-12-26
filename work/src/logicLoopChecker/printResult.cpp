#include "printResult.hpp"
#include <fstream>
#include <vector>
#include <thread>
#include <algorithm>

printResult::printResult(const std::vector<Gate>& gates, const std::vector<std::vector<int>>& graph, const std::vector<sccInfo>& sccInfos) {
    this->gates = gates;
    this->graph = graph;
    this->sccInfos = sccInfos;
    sccResults.resize(sccInfos.size());
}

void printResult::calculateAllSCCs() {
    // int numThreads = std::thread::hardware_concurrency();
    int numThreads = 4;
    std::vector<std::thread> threads(numThreads);
    int chunkSize = sccInfos.size() / numThreads;
    int remainder = sccInfos.size() % numThreads;
    if (remainder != 0) {
        chunkSize++;
    }

    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = std::min((i + 1) * chunkSize, static_cast<int>(sccInfos.size()));
        threads[i] = std::thread(&printResult::calculateSCCs, this, start, end);
        // calculateSCCs(start, end);
    }

    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    printAllSCCs();
}

std::vector<std::vector<int>> printResult::buildSubGraph(const std::vector<int>& scc) {
    std::vector<std::vector<int>> subGraph(graph.size());
    for (int i = 0; i < scc.size(); ++i) {
        for (int j = 0; j < scc.size(); ++j) {
            if (std::find(graph[scc[i]].begin(), graph[scc[i]].end(), scc[j]) != graph[scc[i]].end()) {
                subGraph[scc[i]].push_back(scc[j]);
            }
        }
    }
    return subGraph;
}

void printResult::printAllSCCs() {
    std::ofstream outFile1("result_1.txt");
    for (int i = 0; i < sccInfos.size(); ++i) {
        outFile1 << i + 1 << ")" << std::endl;
        outFile1 << "  Loop Signals: ";
        for (int j = 0; j < sccInfos[i].loopSignals.size(); ++j) {
            outFile1 << sccInfos[i].loopSignals[j] << ", ";
        }
        outFile1 << std::endl;
        outFile1 << "  Loop Gates: ";
        for (int j = 0; j < sccInfos[i].loopGates.size(); ++j) {
            outFile1 << sccInfos[i].loopGates[j] << ", ";
        }
        outFile1 << std::endl << std::endl;
    }
    outFile1.close();

    std::ofstream outFile2("result_2.txt");
    std::ofstream outFile3("result_3.txt");
    std::ofstream outFile4("result_4.txt");
    int resultCount2 = 0;
    int resultCount3 = 0;
    for (int i = 0; i < sccInfos.size(); ++i) {
        if (!sccResults[i].osc) {
            resultCount2++;
            outFile2 << resultCount2 << ")" << std::endl;
            outFile2 << "  Loop Signals: ";
            for (int j = 0; j < sccInfos[i].loopSignals.size(); ++j) {
                outFile2 << sccInfos[i].loopSignals[j] << ", ";
            }
            outFile2 << std::endl;
            outFile2 << "  Loop Gates: ";
            for (int j = 0; j < sccInfos[i].loopGates.size(); ++j) {
                outFile2 << sccInfos[i].loopGates[j] << ", ";
            }
            outFile2 << std::endl << std::endl;
        }
        else {
            resultCount3++;
            outFile3 << resultCount3 << ")" << std::endl;
            outFile3 << "  Loop Signals: ";
            for (int j = 0; j < sccInfos[i].loopSignals.size(); ++j) {
                outFile3 << sccInfos[i].loopSignals[j] << ", ";
            }
            outFile3 << std::endl;
            outFile3 << "  Loop Gates: ";
            for (int j = 0; j < sccInfos[i].loopGates.size(); ++j) {
                outFile3 << sccInfos[i].loopGates[j] << ", ";
            }
            outFile3 << std::endl;
            for (int j = 0; j < sccResults[i].conditions.size(); ++j) {
                outFile3 << "  Loop Condition: ";
                for (int k = 0; k < sccResults[i].conditions[j].size(); ++k) {
                    outFile3 << sccResults[i].conditions[j][k] << ", ";
                }
                outFile3 << std::endl;
            }
            outFile3 << std::endl;

            outFile4 << resultCount3 << ")" << std::endl;
            outFile4 << "  Loop Breaker: ";
            for (int j = 0; j < sccResults[i].breakers.size(); ++j) {
                outFile4 << sccResults[i].breakers[j] << ", ";
            }
            outFile4 << std::endl << std::endl;
        }
    }
    outFile2.close();
    outFile3.close();
    outFile4.close();
}

void printResult::calculateSCCs(int start, int end) {
    for (int i = start; i < end; ++i) {
        oscillationSCC oscSCC(gates, graph, sccInfos[i]);
        std::vector<std::vector<condition>> oscillationConditions = oscSCC.judgeOscillation();
        if (oscillationConditions.size() == 0) {
            sccResults[i].osc = false;
        }
        else {
            sccResults[i].osc = true;
            for (int j = 0; j < oscillationConditions.size(); ++j) {
                sccResults[i].conditions.push_back(std::vector<std::string>());
                for (int k = 0; k < oscillationConditions[j].size(); ++k) {
                    sccResults[i].conditions[j].push_back(gates[oscillationConditions[j][k].gateIndex].name + ".port" + std::to_string(oscillationConditions[j][k].portIndex) + "=" + std::to_string(oscillationConditions[j][k].value));
                }
                std::sort(sccResults[i].conditions[j].begin(), sccResults[i].conditions[j].end());
            }
            
            std::unordered_set<std::string> breakers = oscSCC.minBreakers();
            for (auto it = breakers.begin(); it != breakers.end(); ++it) {
                sccResults[i].breakers.push_back(*it);
            }
            std::sort(sccResults[i].breakers.begin(), sccResults[i].breakers.end());
        }
    }
}
