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
    // Calculate acceleration based on the following distance and speed
    if (road->hasLeadingVehicle(this)) {
        // Get the position and speed of the leading vehicle
        Vehicle* leading_vehicle = road->getLeadingVehicle(this);
        double delta_x = leading_vehicle->getPosition() - position - l;
        double delta_v = speed - leading_vehicle->getSpeed();

        // Calculate interaction term δ
        double delta = F_MIN + std::max(0.0, (speed + delta_v) / (2 * std::sqrt(amax * bmax))) * delta_x;
        acceleration = amax * (1 - std::pow(speed / vmax, 4) - std::pow(delta, 2));
    } else {
        // If no leading vehicle, vehicle accelerates freely
        acceleration = amax * (1 - std::pow(speed / vmax, 4));
    }
}

void Vehicle::update(double deltaTime) {
    if (speed + acceleration * deltaTime < 0) {
        position -= (speed * speed) / (2 * acceleration);  // Adjust position if speed goes negative
        speed = 0;  // Set speed to zero
    } else {
        speed = std::min(speed + acceleration * deltaTime, vmax);  // Update speed
        position += speed * deltaTime + (acceleration * deltaTime * deltaTime) / 2;  // Update position
    }
}

void Vehicle::applyTrafficLightRules() {
    for (auto* light : road->getTrafficLights()) {
        if (!light->isGreen() && light->getPosition() > position &&
            light->getPosition() - position < xs0) {
            // Smooth deceleration if near a red light
            double distanceToLight = light->getPosition() - position;
            acceleration = std::min(-bmax, -std::pow(distanceToLight / xs0, 2) * amax);
            }
    }
}


double Vehicle::getSpeed() const {
    return speed;
}