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

Simulation::Simulation()
    : currentTime(0), stepCounter(0), vehicleCounter(1) {
}

void Simulation::loadFromFile(const std::string& filename) {
    Parser::parseFile(filename, roads, generators, busStops, intersections);
}

void Simulation::runStep() {
    for (auto* road : roads) {
        for (auto* vehicle : road->getVehicles()) {
            if (vehicle == nullptr) continue;

            vehicle->calculateAcceleration();
            vehicle->applyTrafficLightRules();

            bool isWaiting = false;
            for (auto* busStop : busStops) {
                if (busStop->getRoadName() == road->getName()
                    && std::abs(vehicle->getPosition() - busStop->getPosition()) < 1.0
                    && vehicle->getType() == "bus") {
                    isWaiting = vehicle->shouldWaitAt(busStop->getPosition(), busStop->getWaitTime());
                    break;
                    }
            }

            if (!isWaiting) {
                vehicle->update(0.0166);
            }

            for (auto* intersection : intersections) {
                intersection->handleRoadSwitch(vehicle);
            }

            if (vehicle->getRoad() == road && vehicle->getPosition() >= road->getLength()) {
                road->removeVehicle(vehicle);
            }
        }

        for (auto* light : road->getTrafficLights()) {
            light->update(currentTime);
        }
    }

    for (auto* generator : generators) {
        generator->update(currentTime);
    }

    currentTime += 0.0166;
    stepCounter++;
}

void Simulation::run() {
    while (true) {
        runStep();

        bool allRoadsEmpty = true;
        for (auto* road : roads) {
            if (!road->getVehicles().empty()) {
                allRoadsEmpty = false;
                break;
            }
        }

        if (allRoadsEmpty) {
            std::cout << "Simulation ended, no vehicles on roads" << std::endl;
            break;
        }

        outputState();
    }
}


void Simulation::outputState() const {
    int vehicleCounter = 1;

    std::cout << "Increment: " << stepCounter << std::endl;
    std::cout << "Tijd: " << currentTime << std::endl
    << "\n";

    for (const Road* road : roads) {
        if (!road->getVehicles().empty())
            std::cout << "Baan: " << road->getName() << "\n" << std::endl;
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

        for (const TrafficLight* light : road->getTrafficLights()) {
            if (!road->getVehicles().empty()) {
                std::cout << "Verkeerslicht is "
                          << (light->isGreen() ? "groen" : "rood")
                          << "\n" << std::endl;
            }
        }
    }
    std::cout << "-----------------------------------" << std::endl;
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

    for (auto* busStop : busStops) {
        delete busStop;
    }

    for (auto* intersection : intersections) {
        delete intersection;
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
