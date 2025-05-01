#include "Vehicle.h"
#include "Road.h"
#include <cmath>
#include "TrafficLight.h"
#include "Vehicle.h"
#include <algorithm>

const double Vehicle::l    = 4;
const double Vehicle::Vmax = 16.6;
const double Vehicle::amax = 1.44;
const double Vehicle::bmax = 4.61;
const double Vehicle::F_MIN = 4;
const double Vehicle::dt   = 0.0166;
const double Vehicle::xs   = 50;
const double Vehicle::xs0  = 15;
const double Vehicle::s    = 0.4;

Vehicle::Vehicle(Road* road, double position)
    : road(road), position(position), speed(0), acceleration(0), vmax(Vmax) {}

double Vehicle::getPosition() const { return position; }

void Vehicle::calculateAcceleration() {
    if (road->hasLeadingVehicle(this)) {
        Vehicle* leading_vehicle = road->getLeadingVehicle(this);
        double delta_x = leading_vehicle->getPosition() - position - l;
        double delta_v = speed - leading_vehicle->getSpeed();

        double delta = F_MIN + std::max(0.0, (speed + delta_v) / (2 * std::sqrt(amax * bmax))) * delta_x;
        acceleration = amax * (1 - std::pow(speed / vmax, 4) - std::pow(delta, 2));
    } else {
        acceleration = amax * (1 - std::pow(speed / vmax, 4));
    }
}

void Vehicle::update(double deltaTime) {
    if (speed + acceleration * deltaTime < 0) {
        position -= (speed * speed) / (2 * acceleration);
        speed = 0;
    } else {
        speed = std::min(speed + acceleration * deltaTime, vmax);
        position += speed * deltaTime + (acceleration * deltaTime * deltaTime) / 2;
    }
}

void Vehicle::applyTrafficLightRules() {
    for (auto* light : road->getTrafficLights()) {
        if (!light->isGreen() && light->getPosition() > position &&
            light->getPosition() - position < xs0) {
            double distanceToLight = light->getPosition() - position;
            acceleration = std::min(-bmax, -std::pow(distanceToLight / xs0, 2) * amax);
            }
    }
}


double Vehicle::getSpeed() const {
    return speed;
}