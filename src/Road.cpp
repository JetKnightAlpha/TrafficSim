#include "Road.h"

Road::Road(const std::string &name, double length)
    : fName(name), fLength(length) {}

Road::Road(double length)
    : fName("test_road"), fLength(length) {}

void Road::addVehicle(const Vehicle& vehicle) {
    fVehicles.push_back(vehicle); // copy
}

void Road::simulateStep(double deltaTime) {
    for (auto& vehicle : fVehicles) {
        vehicle.update(deltaTime);
    }
}

const std::vector<Vehicle>& Road::getVehicles() const {
    return fVehicles;
}

std::string Road::getName() const {
    return fName;
}

double Road::getLength() const {
    return fLength;
}
