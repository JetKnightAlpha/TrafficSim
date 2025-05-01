#include "Simulation.h"
#include "Parser.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"
#include <iostream>

Simulation::Simulation()
    : currentTime(0), stepCounter(0), vehicleCounter(1) {
}

void Simulation::loadFromFile(const std::string& filename) {
    Parser::parseFile(filename, roads, generators);
}

void Simulation::runStep() {
    for (auto* road : roads)
        for (auto* vehicle : road->getVehicles())
            vehicle->calculateAcceleration(), vehicle->update(0.0166);

    for (auto* road : roads)
        for (auto* light : road->getTrafficLights())
            light->update(currentTime);

    for (auto* generator : generators)
        generator->update(currentTime);

    currentTime += 0.0166;
    stepCounter++;
}

void Simulation::run() {
    for (int i = 0; i < 1000; ++i) {
        runStep();
        outputState();
    }
}

void Simulation::outputState() const {
    int vehicleCounter = 1;

    std::cout << "Tijd: " << stepCounter << std::endl;

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
            vehicleCounter++;
        }

        for (const TrafficLight* light : road->getTrafficLights()) {
            std::cout << "Verkeerslicht is "
                      << (light->isGreen() ? "groen" : "rood")
                      << "\n" << std::endl;
        }

        std::cout << "-----------------------------------" << std::endl;
    }
}

Simulation::~Simulation() {
    for (auto* road : roads) {
        for (auto* vehicle : road->getVehicles())
            delete vehicle;
        for (auto* light : road->getTrafficLights())
            delete light;
        delete road;
    }

    for (auto* gen : generators) {
        delete gen;
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
