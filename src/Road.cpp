#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "BusStop.h"
#include "Intersection.h"
#include <limits>
#include <iostream>
#include <vector>
#include <algorithm>

// Constructor initializes road with a name and a minimum length of 100
Road::Road(const std::string& name, int length) {
    this->name = name;
    this->length = std::max(length, 100);
}


// Returns the name of the road
const std::string& Road::getName() const {
    return name;
}

// Returns the length of the road
int Road::getLength() const {
    return length;
}

// Adds a vehicle to the road
void Road::addVehicle(Vehicle* vehicle) {
    vehicles.push_back(vehicle);
}

// Adds a traffic light to the road
void Road::addTrafficLight(TrafficLight* light) {
    lights.push_back(light);
}

// Adds a connected road
void Road::addRoad(Road* road) {
    roads.push_back(road);
}

// Adds a bus stop to the road
void Road::addBusStop(BusStop* stop) {
    busStops.push_back(stop);
}

// Adds an intersection to the road
void Road::addIntersection(Intersection* intersection) {
    intersections.push_back(intersection);
}

// Simulates per road
void Road::update() {
    for (auto* vehicle : getVehicles()) {
        if (vehicle == nullptr) continue;

        // Update vehicle's acceleration and check traffic lights
        vehicle->calculateAcceleration();
        vehicle->applyTrafficLightRules();

        // Check if a bus needs to wait at a bus stop
        bool isWaiting = false;
        for (auto* busStop : busStops) {
            if (busStop->getRoadName() == getName()
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
        if (vehicle->getPosition() >= getLength()) {
            removeVehicle(vehicle);
        }
    }
}

// Returns the list of vehicles on the road
const std::vector<Vehicle*>& Road::getVehicles() const {
    return vehicles;
}

// Returns the list of traffic lights on the road
const std::vector<TrafficLight*>& Road::getTrafficLights() const {
    return lights;
}

// Returns the list of connected roads
const std::vector<Road*>& Road::getRoads() const {
    return roads;
}

// Checks if there is a vehicle ahead of the given vehicle
bool Road::hasLeadingVehicle(const Vehicle* vehicle) const {
    for (auto* v : vehicles) {
        if (v->getPosition() > vehicle->getPosition()) {
            return true;
        }
    }
    return false;
}

// Returns the closest vehicle ahead of the given vehicle
Vehicle* Road::getLeadingVehicle(const Vehicle* vehicle) const {
    Vehicle* closest = nullptr;
    double minDist = std::numeric_limits<double>::max();
    for (auto* v : vehicles) {
        double gap = v->getPosition() - vehicle->getPosition();
        if (gap > 0 && gap < minDist) {
            minDist = gap;
            closest = v;
        }
    }
    return closest;
}

// Removes the given vehicle from the road
void Road::removeVehicle(Vehicle* vehicle) {
    auto it = std::find(vehicles.begin(), vehicles.end(), vehicle);
    if (it != vehicles.end()) {
        vehicles.erase(it);
    }
}

// Searches for and returns a road by name from a list of roads
Road* Road::getRoadByName(const std::string& roadName, const std::vector<Road*>& roads) {
    for (auto* road : roads) {
        if (road->getName() == roadName) {
            return road;
        }
    }
    return nullptr;
}
