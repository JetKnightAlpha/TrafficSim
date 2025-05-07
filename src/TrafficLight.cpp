#include "TrafficLight.h"

// Constructor initializes traffic light with road, position, and cycle time
TrafficLight::TrafficLight(Road* road, double position, int cycle)
    : road(road), position(position), cycle(cycle), green(true), lastSwitchTime(0) {}

// Updates the traffic light state based on time (green/red cycle)
void TrafficLight::update(double time) {
    if (time - lastSwitchTime >= cycle) {
        green = !green;
        lastSwitchTime = time;
    }
}

// Returns whether the traffic light is green
bool TrafficLight::isGreen() const {
    return green;
}

// Returns the position of the traffic light
double TrafficLight::getPosition() const {
    return position;
}

// Returns the status (green or red) of the traffic light
std::string TrafficLight::getStatus() const {
    return green ? "green" : "red";
}
