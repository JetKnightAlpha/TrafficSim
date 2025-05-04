#include "Vehicle.h"
#include "Road.h"
#include "TrafficLight.h"
#include <cmath>
#include <algorithm>
#include <unordered_map>

const double l    = 4;
const double Vmax = 16.6;
const double amax = 1.44;
const double bmax = 4.61;
const double F_MIN = 4;
const double xs0  = 15;

Vehicle::Vehicle(Road* road, double position, const std::string& type)
    : road(road), position(position), speed(0), acceleration(0), vmax(Vmax), type(type) {}

double Vehicle::getPosition() const { return position; }
double Vehicle::getSpeed() const { return speed; }
const std::string& Vehicle::getType() const { return type; }
double Vehicle::getAcceleration() const { return acceleration; }

void Vehicle::calculateAcceleration() {
    if (road->hasLeadingVehicle(this)) {
        Vehicle* lead = road->getLeadingVehicle(this);
        double delta_x = lead->getPosition() - position - l;
        double delta_v = speed - lead->getSpeed();

        double safe_gap = F_MIN + std::max(0.0, (speed + delta_v) / (2 * std::sqrt(amax * bmax))) * delta_x;
        acceleration = amax * (1 - std::pow(speed / vmax, 4) - std::pow(safe_gap / delta_x, 2));
    } else {
        acceleration = amax * (1 - std::pow(speed / vmax, 4));
    }
}

void Vehicle::applyTrafficLightRules() {
    for (auto* light : road->getTrafficLights()) {
        int lightPos = light->getPosition();
        if (!light->isGreen() && lightPos > position && lightPos - position < xs0) {
            double distanceToLight = lightPos - position;
            acceleration = std::min(-bmax, -std::pow(distanceToLight / xs0, 2) * amax);
        }
    }
}

void Vehicle::update(double deltaTime) {
    if (speed + acceleration * deltaTime < 0) {
        position -= (speed * speed) / (2 * acceleration);
        speed = 0;
    } else {
        speed = std::min(speed + acceleration * deltaTime, vmax);
        position += speed * deltaTime + 0.5 * acceleration * deltaTime * deltaTime;
    }
}

bool Vehicle::shouldWaitAt(double stopPos, double waitDuration) {
    static std::unordered_map<double, double> waitTimers;
    double distance = std::fabs(this->getPosition() - stopPos);
    if (distance < 0.5) {
        if (waitTimers[stopPos] < waitDuration) {
            waitTimers[stopPos] += 0.0166;
            this->speed = 0;
            return true;
        }
    } else {
        waitTimers[stopPos] = 0;
    }
    return false;
}