#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

class Road;
class VehicleGenerator;
class BusStop;
class Intersection;

/**
 * @brief Utility class responsible for parsing XML input files to build the simulation elements.
 * 
 * Provides a static method to parse a scenario file and fill the passed vectors
 * with dynamically allocated objects representing roads, vehicle generators, bus stops, and intersections.
 */
class Parser {
public:
    /**
     * @brief Parses an XML file to extract simulation data.
     * 
     * This method reads the provided XML scenario file and populates the provided
     * vectors with pointers to newly created simulation objects like roads, vehicle generators,
     * bus stops, and intersections.
     * 
     * @param filename Path to the XML input file.
     * @param roads Vector to be filled with pointers to Road objects.
     * @param generators Vector to be filled with pointers to VehicleGenerator objects.
     * @param busStops Vector to be filled with pointers to BusStop objects.
     * @param intersections Vector to be filled with pointers to Intersection objects.
     * 
     * @throws std::runtime_error if the file cannot be opened, or if XML parsing fails,
     *         or if invalid data is encountered.
     */
    static void parseFile(const std::string& filename,
                          std::vector<Road*>& roads,
                          std::vector<VehicleGenerator*>& generators,
                          std::vector<BusStop*>& busStops,
                          std::vector<Intersection*>& intersections);
};

#endif
