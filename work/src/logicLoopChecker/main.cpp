#include "gate.hpp"
#include "parser.hpp"
#include "tarjan.hpp"
#include "sccInfo.hpp"
#include "loopFinder.hpp"
#include "oscillationSCC.hpp"
#include "printResult.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <modinfo.txt>" << std::endl;
        return 1;
    }

    std::vector<Gate> gates;
    const std::string filename = argv[1];
    std::vector<std::vector<int>> graph;

    auto start = std::chrono::high_resolution_clock::now();
    parseVerilogInfo(filename, gates);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Parse Verilog Info: " << elapsed.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    buildGraph(gates, graph);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Graph built!" << std::endl;
    std::cout << "Build Graph: " << elapsed.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    Tarjan tarjan(graph);
    std::vector<std::vector<int>> sccs = tarjan.findStronglyConnectedComponents();
    std::vector<sccInfo> sccInfos = saveSCCs(gates, sccs);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "SCCs found!" << std::endl;
    std::cout << "Find SCCs: " << std::chrono::duration<double>(end - start).count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    printResult printRes(gates, graph, sccInfos);
    printRes.calculateAllSCCs();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Print results: " << std::chrono::duration<double>(end - start).count() << " seconds" << std::endl;

    return 0;
}