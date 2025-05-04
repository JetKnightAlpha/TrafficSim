#include "Intersection.h"
#include "Vehicle.h"
#include "Road.h"
#include <iostream>
#include <random>

Intersection::Intersection(const std::vector<EntryExit>& entryExits)
    : entryExits(entryExits) {
}

bool Intersection::isOn(const Vehicle& v) const {
    return std::abs(v.getPosition() - entryExits[0].entryPosition) < 1.0;
}

void Intersection::setRoads(const std::vector<Road*>& allRoads) {
    roads = allRoads;
}

void Intersection::handleRoadSwitch(Vehicle& vehicle) {
    Road* currentRoad = const_cast<Road*>(vehicle.getRoad());

    if (currentRoad) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        int decision = dis(gen);

        if (decision == 1) {
            for (const auto& entryExit : entryExits) {
                if (currentRoad->getName() == entryExit.entryRoad) {
                    Road * newRoad = Road::getRoadByName(entryExit.exitRoad, roads);

                    if (newRoad) {
                        vehicle.setRoad(newRoad);
                        vehicle.setSpeed(0);
                        vehicle.setPosition(entryExit.exitPosition);
                        break;
                    }
                } else {
                    vehicle.setPosition(vehicle.getPosition());
                    vehicle.setSpeed(vehicle.getSpeed());
                }
            }
        }
    }
}