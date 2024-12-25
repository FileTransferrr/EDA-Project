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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <modinfo.txt>" << std::endl;
        return 1;
    }

    std::vector<Gate> gates;
    const std::string filename = argv[1];
    std::vector<std::vector<int>> graph;
    parseModInfo(filename, gates);
    buildGraph(gates, graph);
    std::cout << "Graph built!" << std::endl;
    Tarjan tarjan(graph);
    std::vector<std::vector<int>> sccs = tarjan.findStronglyConnectedComponents();
    std::vector<sccInfo> sccInfos = saveSCCs(gates, sccs);
    std::cout << "SCCs found!" << std::endl;
    printResult printRes(gates, graph, sccInfos);
    printRes.calculateAllSCCs();    

    return 0;
}