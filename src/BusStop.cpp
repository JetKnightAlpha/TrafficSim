#include "BusStop.h"
#include "Vehicle.h"

// Constructor for BusStop, initializes the road name, position, and wait time
BusStop::BusStop(const std::string& roadName, double position, double waitTime)
    : roadName(roadName), position(position), waitTimeSeconds(waitTime) {}

// Returns the name of the road the bus stop is on
const std::string& BusStop::getRoadName() const {
    return roadName;
}

// Returns the position of the bus stop on the road
double BusStop::getPosition() const {
    return position;
}

// Returns how long a bus stops here in seconds
double BusStop::getWaitTime() const {
    return waitTimeSeconds;
}
