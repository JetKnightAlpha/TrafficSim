#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

class Road;
class VehicleGenerator;
class BusStop;
class Intersection;

class Parser {
public:
    static void parseFile(const std::string& filename,
                          std::vector<Road*>& roads,
                          std::vector<VehicleGenerator*>& generators,
                          std::vector<BusStop*>& busStops,
                          std::vector<Intersection*>& intersections);
};

#endif
