#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include <limits>
#include <iostream>
#include <vector>
#include <algorithm>

Road::Road(const std::string& name, int length) {
    this->name = name;
    this->length = std::max(length, 100);
}

const std::string& Road::getName() const {
    return name;
}

int Road::getLength() const {
    return length;
}

void Road::addVehicle(Vehicle* vehicle) {
    vehicles.push_back(vehicle);
}

void Road::addTrafficLight(TrafficLight* light) {
    lights.push_back(light);
}

void Road::addRoad(Road* road) {
    roads.push_back(road);
}

const std::vector<Vehicle*>& Road::getVehicles() const {
    return vehicles;
}

const std::vector<TrafficLight*>& Road::getTrafficLights() const {
    return lights;
}

const std::vector<Road*>& Road::getRoads() const {
    return roads;
}

bool Road::hasLeadingVehicle(const Vehicle* vehicle) const {
    for (auto* v : vehicles) {
        if (v->getPosition() > vehicle->getPosition()) {
            return true;
        }
    }
    return false;
}

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

void Road::removeVehicle(Vehicle* vehicle) {
    auto it = std::find(vehicles.begin(), vehicles.end(), vehicle);
    if (it != vehicles.end()) {
        vehicles.erase(it);
    }
}

Road* Road::getRoadByName(const std::string& roadName, const std::vector<Road*>& roads) {
    for (auto* road : roads) {
        if (road->getName() == roadName) {
            return road;
        }
    }
    return nullptr;
}