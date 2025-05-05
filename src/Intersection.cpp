#include "Intersection.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

Intersection::Intersection(Road* road1, double pos1, Road* road2, double pos2) {
    roads.first = {road1, pos1};
    roads.second = {road2, pos2};

    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }
}

void Intersection::handleRoadSwitch(Vehicle* vehicle) {
    Road* currentRoad = const_cast<Road*>(vehicle->getRoad());
    double vehiclePos = vehicle->getPosition();

    auto& entry = roads.first;
    auto& exit = roads.second;

    if (currentRoad == entry.road && std::abs(vehiclePos - entry.position) < 1.0) {
        if ((std::rand() % 100) < 30) {
            entry.road->removeVehicle(vehicle);
            exit.road->addVehicle(vehicle);
            vehicle->setRoad(exit.road);
            vehicle->setPosition(exit.position);
        }
    }
}
