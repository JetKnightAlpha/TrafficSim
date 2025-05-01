#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include <limits>


Road::Road(const std::string& name, int length) {
    this->name = name;
    this->length = std::max(length, 100);  // Ensure minimum road length is reasonable
}

const std::string& Road::getName() const { return name; }
int Road::getLength() const { return length; }

void Road::addVehicle(Vehicle* vehicle) {
    vehicles.push_back(vehicle);
}

void Road::addTrafficLight(TrafficLight* light) {
    lights.push_back(light);
}

const std::vector<Vehicle*>& Road::getVehicles() const { return vehicles; }
const std::vector<TrafficLight*>& Road::getTrafficLights() const { return lights; }

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