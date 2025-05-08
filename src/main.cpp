#include "Simulation.h"
#include "Parser.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"
#include "BusStop.h"
#include "Intersection.h"

// Main function: initializes the simulation, loads input from file, and runs the simulation loop
#include "Simulation.h"
#include "Parser.h"
#include "Road.h"
#include "VehicleGenerator.h"
#include "BusStop.h"
#include "Intersection.h"

int main() {
    std::string filename = "../tests/test_files/test_input.xml";

    std::vector<Road*> roads;
    std::vector<VehicleGenerator*> generators;
    std::vector<BusStop*> busStops;
    std::vector<Intersection*> intersections;

    // Parse the scenario file directly in main
    Parser::parseFile(filename, roads, generators, busStops, intersections);

    // Create simulation
    Simulation sim;

    // Add parsed elements to the simulation
    for (auto* road : roads)
        sim.addRoad(road);

    for (auto* gen : generators)
        sim.addGenerator(gen);

    for (auto* bs : busStops)
        sim.addBusStop(bs);

    for (auto* isec : intersections)
        sim.addIntersection(isec);

    // Add vehicles and lights directly from roads
    for (auto* road : roads) {
        for (auto* vehicle : road->getVehicles())
            sim.addVehicle(vehicle);
        for (auto* light : road->getTrafficLights())
            sim.addTrafficLight(light);
    }

    // Run the simulation
    sim.run();

    return 0;
}
