#include "BusStop.h"
#include "Vehicle.h"
#include "DesignByContract.h"

/**
 * @brief Constructs a BusStop object with the specified road name, position, and wait time.
 * 
 * @param roadName The name of the road where the bus stop is located.
 * @param position The position of the bus stop along the road (must be non-negative).
 * @param waitTime The time in seconds that a bus waits at this stop (must be non-negative).
 */
BusStop::BusStop(const std::string& roadName, double position, double waitTime)
    : roadName(roadName), position(position), waitTimeSeconds(waitTime) 
{
    REQUIRE(!roadName.empty(), "roadName must not be empty");
    REQUIRE(position >= 0.0, "position must be non-negative");
    REQUIRE(waitTime >= 0.0, "waitTime must be non-negative");
    
    ENSURE(this->roadName == roadName, "roadName must be properly set");
    ENSURE(this->position == position, "position must be properly set");
    ENSURE(this->waitTimeSeconds == waitTime, "waitTime must be properly set");
}

/**
 * @brief Gets the name of the road where the bus stop is located.
 * 
 * @return const std::string& The road name.
 */
const std::string& BusStop::getRoadName() const {
    ENSURE(!roadName.empty(), "returned road name must not be empty");
    return roadName;
}

/**
 * @brief Gets the position of the bus stop on the road.
 * 
 * @return double The position along the road (non-negative).
 */
double BusStop::getPosition() const {
    ENSURE(position >= 0.0, "returned position must be non-negative");
    return position;
}

/**
 * @brief Gets the wait time for a bus at this stop.
 * 
 * @return double The wait time in seconds (non-negative).
 */
double BusStop::getWaitTime() const {
    ENSURE(waitTimeSeconds >= 0.0, "returned wait time must be non-negative");
    return waitTimeSeconds;
}
