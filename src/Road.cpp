#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "BusStop.h"
#include "Intersection.h"
#include "DesignByContract.h"
#include <limits>
#include <iostream>
#include <vector>
#include <algorithm>

/**
 * @brief Constructs a road with a given name and length.
 * 
 * The length is guaranteed to be at least 100.
 * 
 * @param name The name of the road. Must not be empty.
 * @param length The length of the road. Must be positive.
 */
Road::Road(const std::string& name, int length) {
    REQUIRE(!name.empty(), "Road name cannot be empty");
    REQUIRE(length > 0, "Road length must be positive");
    
    this->name = name;
    this->length = std::max(length, 100);
    
    ENSURE(!this->name.empty(), "Road name was not set properly");
    ENSURE(this->length >= 100, "Road length must be at least 100");
}

/**
 * @brief Returns the name of the road.
 * 
 * @return const std::string& The name of the road.
 */
const std::string& Road::getName() const {
    ENSURE(!name.empty(), "Road name should not be empty");
    return name;
}

/**
 * @brief Returns the length of the road.
 * 
 * @return int The length of the road.
 */
int Road::getLength() const {
    ENSURE(length >= 100, "Road length should be at least 100");
    return length;
}

/**
 * @brief Adds a vehicle to this road.
 * 
 * @param vehicle Pointer to the vehicle to add. Must not be null.
 */
void Road::addVehicle(Vehicle* vehicle) {
    REQUIRE(vehicle != nullptr, "Vehicle cannot be null");
    
    size_t oldSize = vehicles.size();
    vehicles.push_back(vehicle);
    
    ENSURE(vehicles.size() == oldSize + 1, "Vehicle was not added properly");
}

/**
 * @brief Adds a traffic light to this road.
 * 
 * @param light Pointer to the traffic light to add. Must not be null.
 */
void Road::addTrafficLight(TrafficLight* light) {
    REQUIRE(light != nullptr, "Traffic light cannot be null");
    
    size_t oldSize = lights.size();
    lights.push_back(light);
    
    ENSURE(lights.size() == oldSize + 1, "Traffic light was not added properly");
}

/**
 * @brief Adds a connected road to this road.
 * 
 * @param road Pointer to the road to add. Must not be null and cannot be this road itself.
 */
void Road::addRoad(Road* road) {
    REQUIRE(road != nullptr, "Road cannot be null");
    REQUIRE(road != this, "Cannot add road to itself");
    
    size_t oldSize = roads.size();
    roads.push_back(road);
    
    ENSURE(roads.size() == oldSize + 1, "Road was not added properly");
}

/**
 * @brief Adds a bus stop to this road.
 * 
 * @param stop Pointer to the bus stop to add. Must not be null.
 */
void Road::addBusStop(BusStop* stop) {
    REQUIRE(stop != nullptr, "Bus stop cannot be null");
    
    size_t oldSize = busStops.size();
    busStops.push_back(stop);
    
    ENSURE(busStops.size() == oldSize + 1, "Bus stop was not added properly");
}

/**
 * @brief Adds an intersection to this road.
 * 
 * @param intersection Pointer to the intersection to add. Must not be null.
 */
void Road::addIntersection(Intersection* intersection) {
    REQUIRE(intersection != nullptr, "Intersection cannot be null");
    
    size_t oldSize = intersections.size();
    intersections.push_back(intersection);
    
    ENSURE(intersections.size() == oldSize + 1, "Intersection was not added properly");
}

/**
 * @brief Updates the state of the road and all vehicles on it.
 * 
 * Performs the following for each vehicle:
 * - Calculates acceleration and applies traffic light rules.
 * - Checks if buses need to wait at bus stops.
 * - Updates vehicle position if not waiting.
 * - Handles road switching at intersections.
 * - Removes vehicles that have reached the end of the road.
 */
void Road::update() {
    for (auto* vehicle : getVehicles()) {
        if (vehicle == nullptr) continue;

        // Update vehicle acceleration and traffic light compliance
        vehicle->calculateAcceleration();
        vehicle->applyTrafficLightRules();

        // Determine if bus must wait at a nearby bus stop
        bool isWaiting = false;
        for (auto* busStop : busStops) {
            if (busStop->getRoadName() == getName()
                && std::abs(vehicle->getPosition() - busStop->getPosition()) < 1.0
                && vehicle->getType() == "bus") {

                isWaiting = vehicle->shouldWaitAt(busStop->getPosition(), busStop->getWaitTime());
                break;
            }
        }

        // Update vehicle's position if it is not waiting
        if (!isWaiting) {
            vehicle->update(0.0166); // Assuming ~60 FPS frame time
        }

        // Handle potential road switching at intersections
        for (auto* intersection : intersections) {
            intersection->handleRoadSwitch(vehicle);
        }

        // Remove vehicle if it has passed the end of the road
        if (vehicle->getPosition() >= getLength()) {
            removeVehicle(vehicle);
        }
    }
}

/**
 * @brief Returns the vector of vehicles currently on the road.
 * 
 * @return const std::vector<Vehicle*>& Vector of vehicle pointers.
 */
const std::vector<Vehicle*>& Road::getVehicles() const {
    return vehicles;
}

/**
 * @brief Returns the vector of traffic lights on the road.
 * 
 * @return const std::vector<TrafficLight*>& Vector of traffic light pointers.
 */
const std::vector<TrafficLight*>& Road::getTrafficLights() const {
    return lights;
}

/**
 * @brief Returns the vector of connected roads.
 * 
 * @return const std::vector<Road*>& Vector of road pointers.
 */
const std::vector<Road*>& Road::getRoads() const {
    return roads;
}

/**
 * @brief Checks if there is a vehicle ahead of the given vehicle on this road.
 * 
 * @param vehicle Pointer to the vehicle to check for leading vehicles. Must not be null.
 * @return true if there is a vehicle ahead, false otherwise.
 */
bool Road::hasLeadingVehicle(const Vehicle* vehicle) const {
    REQUIRE(vehicle != nullptr, "Vehicle cannot be null");
    
    for (auto* v : vehicles) {
        if (v->getPosition() > vehicle->getPosition()) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Finds and returns the closest vehicle ahead of the given vehicle.
 * 
 * @param vehicle Pointer to the vehicle for which to find the leading vehicle. Must not be null.
 * @return Vehicle* Pointer to the closest vehicle ahead, or nullptr if none found.
 */
Vehicle* Road::getLeadingVehicle(const Vehicle* vehicle) const {
    REQUIRE(vehicle != nullptr, "Vehicle cannot be null");
    
    Vehicle* closest = nullptr;
    double minDist = std::numeric_limits<double>::max();
    for (auto* v : vehicles) {
        double gap = v->getPosition() - vehicle->getPosition();
        if (gap > 0 && gap < minDist) {
            minDist = gap;
            closest = v;
        }
    }
    
    if (closest != nullptr) {
        ENSURE(closest->getPosition() > vehicle->getPosition(), "Leading vehicle must be ahead");
    }
    return closest;
}

/**
 * @brief Removes a vehicle from the road.
 * 
 * @param vehicle Pointer to the vehicle to remove. Must not be null.
 */
void Road::removeVehicle(Vehicle* vehicle) {
    REQUIRE(vehicle != nullptr, "Vehicle cannot be null");
    
    size_t oldSize = vehicles.size();
    auto it = std::find(vehicles.begin(), vehicles.end(), vehicle);
    if (it != vehicles.end()) {
        vehicles.erase(it);
        ENSURE(vehicles.size() == oldSize - 1, "Vehicle was not removed properly");
    }
}

/**
 * @brief Searches for a road by name in a list of roads.
 * 
 * @param roadName The name of the road to find. Must not be empty.
 * @param roads Vector of road pointers to search through.
 * @return Road* Pointer to the found road, or nullptr if not found.
 */
Road* Road::getRoadByName(const std::string& roadName, const std::vector<Road*>& roads) {
    REQUIRE(!roadName.empty(), "Road name cannot be empty");
    
    for (auto* road : roads) {
        if (road->getName() == roadName) {
            ENSURE(road != nullptr, "Found road cannot be null");
            ENSURE(road->getName() == roadName, "Found road must have matching name");
            return road;
        }
    }
    return nullptr;
}
