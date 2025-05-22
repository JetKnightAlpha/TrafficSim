#include "Vehicle.h"
#include "Road.h"
#include "TrafficLight.h"
#include "BusStop.h"
#include <cmath>
#include <algorithm>
#include <unordered_map>

// Constants for vehicle behavior
const double l    = 4;          // Vehicle length
const double Vmax = 16.6;       // Maximum speed
const double amax = 1.44;       // Maximum acceleration
const double bmax = 4.61;       // Maximum braking force
const double F_MIN = 4;         // Minimum following distance
const double xs0  = 15;         // Minimum stopping distance

// Constructor initializes vehicle with road, position, type, and other attributes
Vehicle::Vehicle(Road* road, double position)
    : road(road), position(position), speed(0), acceleration(0), vmax(Vmax) {}

// Constructor for auto
Auto::Auto(Road* road, double position) : Vehicle(road, position) {
    type = "auto";
}

// Constructor for bus
Bus::Bus(Road* road, double position) : Vehicle(road, position) {
    type = "bus";
}

// Constructor for politiecombi
Combi::Combi(Road* road, double position) : Vehicle(road, position) {
    type = "politiecombi";
}

// Constructor for ziekenwagen
Ziek::Ziek(Road* road, double position) : Vehicle(road, position) {
    type = "ziekenwagen";
}

// Constructor for brandweerwagen
Brand::Brand(Road* road, double position) : Vehicle(road, position) {
    type = "brandweerwagen";
}

// Calculates the vehicle's acceleration based on the leading vehicle and current state
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

// Applies traffic light rules, slowing down if the vehicle is near a red light
void Vehicle::applyTrafficLightRules() {
    for (auto* light : road->getTrafficLights()) {
        int lightPos = light->getPosition();
        if (!light->isGreen() && lightPos > position && lightPos - position < xs0) {
            double distanceToLight = lightPos - position;
            acceleration = std::min(-bmax, -std::pow(distanceToLight / xs0, 2) * amax);  // Slow down
        }
    }
}

// Updates the vehicle's position and speed based on acceleration and time
void Vehicle::update(double deltaTime) {
    if (speed + acceleration * deltaTime < 0) {
        position -= (speed * speed) / (2 * acceleration);
        speed = 0;
    } else {
        speed = std::min(speed + acceleration * deltaTime, vmax);
        position += speed * deltaTime + 0.5 * acceleration * deltaTime * deltaTime;
    }
}

// Determines if the vehicle should wait at a bus stop
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

// Returns the type of the vehicle
const std::string& Vehicle::getType() const {
    return type;
}

// Returns the position of the vehicle
double Vehicle::getPosition() const {
    return position;
}

// Returns the speed of the vehicle
double Vehicle::getSpeed() const {
    return speed;
}

// Returns the acceleration of the vehicle
double Vehicle::getAcceleration() const {
    return acceleration;
}

const Road* Vehicle::getRoad() const {
    return road;
}

// Sets the road the vehicle is currently on
void Vehicle::setRoad(Road* r) {
    road = r;
}

// Sets the vehicle's position
void Vehicle::setPosition(double newPosition) {
    position = newPosition;
}

// Sets the vehicle's speed
void Vehicle::setSpeed(double newSpeed) {
    speed = newSpeed;
}
