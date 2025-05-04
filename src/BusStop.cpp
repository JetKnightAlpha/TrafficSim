#include "BusStop.h"
#include "Vehicle.h"

BusStop::BusStop(const std::string& roadName, double position, double waitTime)
    : roadName(roadName), position(position), waitTimeSeconds(waitTime) {}

const std::string& BusStop::getRoadName() const {
    return roadName;
}

double BusStop::getPosition() const {
    return position;
}

double BusStop::getWaitTime() const {
    return waitTimeSeconds;
}
