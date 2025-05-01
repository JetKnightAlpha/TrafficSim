#include "TrafficLight.h"

TrafficLight::TrafficLight(Road* road, int position, int cycle)
    : road(road), position(position), cycle(cycle), green(true), lastSwitchTime(0) {}

void TrafficLight::update(double time) {
    if (time - lastSwitchTime > cycle) {
        green = !green;
        lastSwitchTime = time;
    }
}

bool TrafficLight::isGreen() const { return green; }

int TrafficLight::getPosition() const { return position; }

std::string TrafficLight::getStatus() const {
    return green ? "green" : "red"; // Return status as "green" or "red"
}
