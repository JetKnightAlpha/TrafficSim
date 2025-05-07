#include "Simulation.h"
#include "Parser.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"
#include "BusStop.h"
#include "Intersection.h"
#include <iostream>
#include <cmath>

// Constructor initializes simulation state (time, step counter, vehicle counter)
Simulation::Simulation()
    : currentTime(0), stepCounter(0), vehicleCounter(1) {}

// Loads simulation data from a file using the Parser
void Simulation::loadFromFile(const std::string& filename) {
    // Parses roads, generators, bus stops, and intersections from file
    Parser::parseFile(filename, roads, generators, busStops, intersections);

    // Collect vehicles and traffic lights from roads
    for (auto* road : roads) {
        for (auto* vehicle : road->getVehicles()) {
            vehicles.push_back(vehicle);
        }
        for (auto* light : road->getTrafficLights()) {
            trafficLights.push_back(light);
        }
    }
}

// Runs one step of the simulation
void Simulation::runStep() {
    // Iterate over each road to update vehicles, traffic lights, and generators
    for (auto* road : roads) {
        for (auto* vehicle : road->getVehicles()) {
            if (vehicle == nullptr) continue;

            // Update vehicle's acceleration and check traffic lights
            vehicle->calculateAcceleration();
            vehicle->applyTrafficLightRules();

            // Check if a bus needs to wait at a bus stop
            bool isWaiting = false;
            for (auto* busStop : busStops) {
                if (busStop->getRoadName() == road->getName()
                    && std::abs(vehicle->getPosition() - busStop->getPosition()) < 1.0
                    && vehicle->getType() == "bus") {

                    // Determine whether the bus should wait
                    isWaiting = vehicle->shouldWaitAt(busStop->getPosition(), busStop->getWaitTime());
                    break;
                }
            }

            // Update vehicle position if it's not waiting
            if (!isWaiting) {
                vehicle->update(0.0166); // 1 frame ~60 FPS
            }

            // Handle vehicle switching roads at intersections
            for (auto* intersection : intersections) {
                intersection->handleRoadSwitch(vehicle);
            }

            // Remove vehicle if it reached the end of the road
            if (vehicle->getRoad() == road && vehicle->getPosition() >= road->getLength()) {
                road->removeVehicle(vehicle);
            }
        }

        // Update all traffic lights on the road
        for (auto* light : road->getTrafficLights()) {
            light->update(currentTime);
        }

        // Update all vehicle generators for the road
        for (auto* generator : generators) {
            generator->update(currentTime);
        }
    }

    // Advance time and step count
    stepCounter++;
    currentTime += 0.0166;
}

// Runs the simulation until all roads are empty (no vehicles left)
void Simulation::run() {
    while (true) {
        runStep();

        // Check whether all roads have no vehicles
        bool allRoadsEmpty = true;
        for (auto* road : roads) {
            if (!road->getVehicles().empty()) {
                allRoadsEmpty = false;
                break;
            }
        }

        // End simulation if no vehicles remain
        if (allRoadsEmpty) {
            std::cout << "Simulation ended, no vehicles on roads" << std::endl;
            break;
        }

        // Print current state of simulation
        outputState();
    }
}

// Outputs the current state of roads, vehicles, and traffic lights
void Simulation::outputState() const {
    std::cout << "Increment: " << stepCounter << std::endl;
    std::cout << "Tijd: " << currentTime << std::endl << "\n";

    for (const Road* road : roads) {
        int vehicleCounter = 1;

        // Only output if there are vehicles
        if (!road->getVehicles().empty())
            std::cout << "Baan: " << road->getName() << "\n" << std::endl;

        // Output each vehicle on the road
        for (const Vehicle* vehicle : road->getVehicles()) {
            int roundedPosition = static_cast<int>(std::round(vehicle->getPosition()));
            double roundedSpeed = std::round(vehicle->getSpeed() * 10.0) / 10.0;

            std::cout << "Voertuig " << vehicleCounter
                      << std::endl
                      << "-> type: " << vehicle->getType()
                      << std::endl
                      << "-> positie: " << roundedPosition
                      << std::endl
                      << "-> snelheid: " << roundedSpeed
                      << "\n" << std::endl;
            vehicleCounter++;
        }

        // Output state of each traffic light on the road
        for (const TrafficLight* light : road->getTrafficLights()) {
            if (!road->getVehicles().empty()) {
                std::cout << "Verkeerslicht op positie "
                          << light->getPosition()
                          << " is "
                          << (light->isGreen() ? "groen" : "rood")
                          << "\n" << std::endl;
            }
        }
    }
    std::cout << "-----------------------------------" << std::endl;
}

// Destructor deletes all dynamically allocated memory
Simulation::~Simulation() {
    // Delete vehicles and traffic lights for each road, then delete road
    for (auto* road : roads) {
        for (auto* vehicle : road->getVehicles())
            delete vehicle;
        for (auto* light : road->getTrafficLights())
            delete light;
        delete road;
    }

    // Delete all generators
    for (auto* gen : generators) {
        delete gen;
    }

    // Delete all bus stops
    for (auto* busStop : busStops) {
        delete busStop;
    }

    // Delete all intersections
    for (auto* intersection : intersections) {
        delete intersection;
    }
}

// Adds a road to the simulation
void Simulation::addRoad(Road* road) {
    roads.push_back(road);
}

// Adds a traffic light to the simulation
void Simulation::addTrafficLight(TrafficLight* light) {
    trafficLights.push_back(light);
}

// Adds a vehicle to the simulation (manual add, useful for testing)
void Simulation::addVehicle(Vehicle* vehicle) {
    vehicles.push_back(vehicle);
}

// Returns list of roads
const std::vector<Road*>& Simulation::getRoads() const {
    return roads;
}

// Returns list of vehicles
const std::vector<Vehicle*>& Simulation::getVehicles() const {
    return vehicles;
}

// Returns list of traffic lights
const std::vector<TrafficLight*>& Simulation::getTrafficLights() const {
    return trafficLights;
}

// Returns list of intersections
const std::vector<Intersection*>& Simulation::getIntersections() const {
    return intersections;
}
