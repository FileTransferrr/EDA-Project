#ifndef GATE_HPP
#define GATE_HPP

#include <string>
#include <vector>

struct Gate {
    std::string type;
    std::string name;
    std::vector<std::string> inputs;
    std::string output;
};

#endif // GATE_HPP