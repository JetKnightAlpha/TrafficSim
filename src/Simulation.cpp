#include "Simulation.h"
#include "Parser.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"
#include <iostream>

// Constructor
Simulation::Simulation()
    : currentTime(0), stepCounter(0), vehicleCounter(1) {
    // Additional initialization can be done here if needed
}

// Load simulation data from a file
void Simulation::loadFromFile(const std::string& filename) {
    Parser::parseFile(filename, roads, generators);
}

// Run one simulation step
void Simulation::runStep() {
    for (auto* road : roads)
        for (auto* vehicle : road->getVehicles())
            vehicle->calculateAcceleration(), vehicle->update(0.0166); // Update vehicle state

    for (auto* road : roads)
        for (auto* light : road->getTrafficLights())
            light->update(currentTime); // Update traffic lights

    for (auto* generator : generators)
        generator->update(currentTime); // Update vehicle generators

    currentTime += 0.0166;  // Update the current time
    stepCounter++;          // Increment the step counter
}

// Run the simulation for a set number of steps
void Simulation::run() {
    for (int i = 0; i < 1000; ++i) {
        runStep();
        outputState();
    }
}

// Output the current state of the simulation
void Simulation::outputState() const {
    int vehicleCounter = 1; // Reset the vehicle counter for each outputState

    std::cout << "Tijd: " << stepCounter << std::endl;

    // Display vehicles' state
    for (const Road* road : roads) {
        for (const Vehicle* vehicle : road->getVehicles()) {
            std::cout << "Voertuig " << vehicleCounter
                      << std::endl
                      << "-> baan: " << road->getName()
                      << std::endl
                      << "-> positie: " << vehicle->getPosition()
                      << std::endl
                      << "-> snelheid: " << vehicle->getSpeed()
                      << "\n" << std::endl ;
            vehicleCounter++; // Increment the vehicle counter for each vehicle
        }

        // Display traffic light status
        for (const TrafficLight* light : road->getTrafficLights()) {
            std::cout << "Verkeerslicht is "
                      << (light->isGreen() ? "groen" : "rood") // Check if the light is green or red
                      << "\n" << std::endl;
        }

        std::cout << "-----------------------------------" << std::endl;
    }
}

// Destructor to clean up resources
Simulation::~Simulation() {
    for (auto* road : roads) {
        for (auto* vehicle : road->getVehicles())
            delete vehicle;  // Delete each vehicle
        for (auto* light : road->getTrafficLights())
            delete light;     // Delete each traffic light
        delete road;  // Delete the road itself
    }

    for (auto* gen : generators) {
        delete gen;  // Delete each vehicle generator
    }
}

void Simulation::addRoad(Road* road) {
    roads.push_back(road);
}

void Simulation::addTrafficLight(TrafficLight* light) {
    trafficLights.push_back(light);
}

void Simulation::addVehicle(Vehicle* vehicle) {
    vehicles.push_back(vehicle);
}

const std::vector<Road*>& Simulation::getRoads() const {
    return roads;
}

const std::vector<Vehicle*>& Simulation::getVehicles() const {
    return vehicles;
}

const std::vector<TrafficLight*>& Simulation::getTrafficLights() const {
    return trafficLights;
}
