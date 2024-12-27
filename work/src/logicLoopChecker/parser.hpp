#ifndef PARSER_HPP
#define PARSER_HPP

#include "gate.hpp"
#include <string>
#include <vector>
#include <unordered_map>

bool isTargetGate(const std::string& str);

std::string replaceSpecialChars(const std::string& str);

void parseVerilogInfo(const std::string& filename, std::vector<Gate>& gates);

void buildGraph(const std::vector<Gate>& gates, std::vector<std::vector<int>>& graph);

#endif // PARSER_HPP