#include "Intersection.h"
#include "DesignByContract.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

/**
 * @brief Constructs an intersection connecting two distinct roads at specified positions.
 * 
 * Seeds the random number generator the first time an intersection is created.
 * 
 * @param road1 Pointer to the first road.
 * @param pos1 Position along the first road where the intersection occurs.
 * @param road2 Pointer to the second road.
 * @param pos2 Position along the second road where the intersection occurs.
 */
Intersection::Intersection(Road* road1, double pos1, Road* road2, double pos2) {
    REQUIRE(road1 != nullptr, "road1 must not be null");
    REQUIRE(road2 != nullptr, "road2 must not be null");
    REQUIRE(pos1 >= 0.0, "pos1 must be non-negative");
    REQUIRE(pos2 >= 0.0, "pos2 must be non-negative");
    REQUIRE(road1 != road2, "roads must be different");

    roads.first = {road1, pos1};
    roads.second = {road2, pos2};

    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    ENSURE(roads.first.road == road1, "first road must be properly set");
    ENSURE(roads.first.position == pos1, "first position must be properly set");
    ENSURE(roads.second.road == road2, "second road must be properly set");
    ENSURE(roads.second.position == pos2, "second position must be properly set");
}

/**
 * @brief Possibly switches the vehicle to the other road in the intersection.
 * 
 * If the vehicle is within 1.0 unit of the intersection and a random chance (30%) occurs,
 * it is moved to the connected road at the corresponding intersection position.
 * 
 * @param vehicle Pointer to the vehicle being handled.
 */
void Intersection::handleRoadSwitch(Vehicle* vehicle) {
    REQUIRE(vehicle != nullptr, "vehicle must not be null");
    REQUIRE(vehicle->getRoad() != nullptr, "vehicle must be on a road");
    
    Road* originalRoad = const_cast<Road*>(vehicle->getRoad());
    double originalPosition = vehicle->getPosition();
    
    REQUIRE(originalPosition >= 0.0, "vehicle position must be non-negative");

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
            
            ENSURE(vehicle->getRoad() == exit.road, "vehicle must be on the exit road after switch");
            ENSURE(vehicle->getPosition() == exit.position, "vehicle position must be set to exit position");
        }
    }
    
    ENSURE(vehicle != nullptr, "vehicle must still be valid after handling");
    ENSURE(vehicle->getRoad() != nullptr, "vehicle must still be on a road after handling");
}
