#include "Simulation.h"
#include "Parser.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"
#include "BusStop.h"
#include "Intersection.h"

/**
 * @brief Entry point of the traffic simulation program.
 * 
 * This function initializes the simulation environment by:
 * - Loading a scenario XML file
 * - Parsing and constructing roads, vehicle generators, bus stops, and intersections
 * - Adding all components to the Simulation object
 * - Running the main simulation loop
 * 
 * @return int Returns 0 upon successful execution.
 */
int main() {
    /// Path to the XML input file describing the simulation scenario.
    std::string filename = "../tests/test_files/test_input.xml";

    /// Containers for the parsed simulation elements.
    std::vector<Road*> roads;
    std::vector<VehicleGenerator*> generators;
    std::vector<BusStop*> busStops;
    std::vector<Intersection*> intersections;

    /// Parse all simulation elements from the scenario file.
    Parser::parseFile(filename, roads, generators, busStops, intersections);

    /// Create the simulation instance.
    Simulation sim;

    /// Add all parsed roads to the simulation.
    for (auto* road : roads)
        sim.addRoad(road);

    /// Add all vehicle generators to the simulation.
    for (auto* gen : generators)
        sim.addGenerator(gen);

    /// Add all bus stops to the simulation.
    for (auto* bs : busStops)
        sim.addBusStop(bs);

    /// Add all intersections to the simulation.
    for (auto* isec : intersections)
        sim.addIntersection(isec);

    /// Run the simulation loop.
    sim.run();

    return 0;
}
