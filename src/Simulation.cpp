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
            vehicle->update(0.0166);

            for (auto* busStop : busStops) {
                if (vehicle->getRoad()->getName() == busStop->getRoadName() &&
                    std::fabs(vehicle->getPosition() - busStop->getPosition()) < 5) {
                    vehicle->applyTrafficLightRules();
                    vehicle->update(0);
                }
            }

            for (auto* intersection : intersections) {
                if (intersection->isOn(*vehicle)) {
                    auto nextRoad = intersection->getNextRoad(vehicle->getRoad()->getName());
                    if (nextRoad) {
                        const_cast<Road*>(vehicle->getRoad())->removeVehicle(vehicle);
                        Road* nextRoadPtr = Road::getRoadByName(nextRoad->first, roads);
                        if (nextRoadPtr) {
                            nextRoadPtr->addVehicle(vehicle);
                            vehicle->setRoad(nextRoadPtr);
                            vehicle->update(0);
                        }
                    }
                }
            }
        }
    }

    for (auto* road : roads) {
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
    for (int i = 0; i < 1000; ++i) {
        runStep();
        outputState();
    }
}

void Simulation::outputState() const {
    int vehicleCounter = 1;

    std::cout << "Tijd: " << stepCounter << std::endl;

    for (const Road* road : roads) {
        std::cout << "Baan: " << road->getName() << " heeft " << road->getVehicles().size() << " voertuigen" << std::endl;
        for (const Vehicle* vehicle : road->getVehicles()) {
            int roundedPosition = static_cast<int>(std::round(vehicle->getPosition()));
            double roundedSpeed = std::round(vehicle->getSpeed() * 10.0) / 10.0;
            std::cout << "Voertuig " << vehicleCounter
                      << std::endl
                      << "-> type: " << vehicle->getType()
                      << std::endl
                      << "-> baan: " << road->getName()
                      << std::endl
                      << "-> positie: " << roundedPosition
                      << std::endl
                      << "-> snelheid: " << roundedSpeed
                      << "\n" << std::endl;
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
