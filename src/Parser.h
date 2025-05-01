// Parser.h
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
class Road;
class VehicleGenerator;

class Parser {
public:
    static void parseFile(const std::string& filename,
                          std::vector<Road*>& roads,
                          std::vector<VehicleGenerator*>& generators);
};

#endif
