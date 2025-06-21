#include "Vehicle.h"
#include "Road.h"
#include "TrafficLight.h"
#include "BusStop.h"
#include "DesignByContract.h"
#include <cmath>
#include <algorithm>
#include <unordered_map>

// Constants for vehicle behavior
const double l    = 4;          ///< Vehicle length (meters)
const double Vmax = 16.6;       ///< Maximum speed (m/s)
const double amax = 1.44;       ///< Maximum acceleration (m/s^2)
const double bmax = 4.61;       ///< Maximum braking deceleration (m/s^2)
const double F_MIN = 4;         ///< Minimum following distance (meters)
const double xs0  = 15;         ///< Minimum stopping distance before traffic light (meters)

/**
 * @brief Constructs a Vehicle with initial road and position.
 * @param road Pointer to Road (must not be nullptr).
 * @param position Initial position (>= 0).
 * 
 * Ensures speed and acceleration start at zero, and vmax is set.
 */
Vehicle::Vehicle(Road* road, double position)
    : road(road), position(position), speed(0), acceleration(0), vmax(Vmax) {
    REQUIRE(road != nullptr, "Road cannot be null");
    REQUIRE(position >= 0, "Position must be non-negative");

    ENSURE(this->road == road, "Road was not set properly");
    ENSURE(this->position == position, "Position was not set properly");
    ENSURE(this->speed == 0, "Speed should be initialized to 0");
    ENSURE(this->acceleration == 0, "Acceleration should be initialized to 0");
    ENSURE(this->vmax == Vmax, "Max speed should be set to Vmax");
}

/**
 * @brief Constructs an Auto vehicle.
 * @param road Pointer to Road.
 * @param position Initial position.
 */
Auto::Auto(Road* road, double position) : Vehicle(road, position) {
    REQUIRE(road != nullptr, "Road cannot be null");
    REQUIRE(position >= 0, "Position must be non-negative");

    type = "auto";

    ENSURE(type == "auto", "Type should be set to auto");
}

/**
 * @brief Constructs a Bus vehicle.
 * @param road Pointer to Road.
 * @param position Initial position.
 */
Bus::Bus(Road* road, double position) : Vehicle(road, position) {
    REQUIRE(road != nullptr, "Road cannot be null");
    REQUIRE(position >= 0, "Position must be non-negative");

    type = "bus";

    ENSURE(type == "bus", "Type should be set to bus");
}

/**
 * @brief Constructs a Combi (police car).
 * @param road Pointer to Road.
 * @param position Initial position.
 */
Combi::Combi(Road* road, double position) : Vehicle(road, position) {
    REQUIRE(road != nullptr, "Road cannot be null");
    REQUIRE(position >= 0, "Position must be non-negative");

    type = "politiecombi";

    ENSURE(type == "politiecombi", "Type should be set to politiecombi");
}

/**
 * @brief Constructs a Ziek (ambulance) vehicle.
 * @param road Pointer to Road.
 * @param position Initial position.
 */
Ziek::Ziek(Road* road, double position) : Vehicle(road, position) {
    REQUIRE(road != nullptr, "Road cannot be null");
    REQUIRE(position >= 0, "Position must be non-negative");

    type = "ziekenwagen";

    ENSURE(type == "ziekenwagen", "Type should be set to ziekenwagen");
}

/**
 * @brief Constructs a Brand (fire truck) vehicle.
 * @param road Pointer to Road.
 * @param position Initial position.
 */
Brand::Brand(Road* road, double position) : Vehicle(road, position) {
    REQUIRE(road != nullptr, "Road cannot be null");
    REQUIRE(position >= 0, "Position must be non-negative");

    type = "brandweerwagen";

    ENSURE(type == "brandweerwagen", "Type should be set to brandweerwagen");
}

/**
 * @brief Calculates acceleration based on leading vehicle and max acceleration.
 * 
 * If a leading vehicle exists, computes safe gap and adjusts acceleration accordingly.
 * Otherwise, accelerates towards vmax.
 */
void Vehicle::calculateAcceleration() {
    REQUIRE(road != nullptr, "Road cannot be null");
    REQUIRE(speed >= 0, "Speed must be non-negative");

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

/**
 * @brief Applies traffic light rules to slow down when approaching a red light.
 */
void Vehicle::applyTrafficLightRules() {
    REQUIRE(road != nullptr, "Road cannot be null");

    for (auto* light : road->getTrafficLights()) {
        int lightPos = light->getPosition();
        if (!light->isGreen() && lightPos > position && lightPos - position < xs0) {
            double distanceToLight = lightPos - position;
            acceleration = std::min(-bmax, -std::pow(distanceToLight / xs0, 2) * amax);
        }
    }
}

/**
 * @brief Updates position and speed based on acceleration and time delta.
 * @param deltaTime Time elapsed since last update.
 */
void Vehicle::update(double deltaTime) {
    REQUIRE(deltaTime > 0, "Delta time must be positive");
    REQUIRE(position >= 0, "Position must be non-negative");

    double oldPosition = position;
    double oldSpeed = speed;

    if (speed + acceleration * deltaTime < 0) {
        position -= (speed * speed) / (2 * acceleration);
        speed = 0;
    } else {
        speed = std::min(speed + acceleration * deltaTime, vmax);
        position += speed * deltaTime + 0.5 * acceleration * deltaTime * deltaTime;
    }

    ENSURE(speed >= 0, "Speed must remain non-negative");
    ENSURE(speed <= vmax, "Speed cannot exceed maximum speed");
}

/**
 * @brief Determines if vehicle should wait at a bus stop.
 * @param stopPos Position of the bus stop.
 * @param waitDuration Duration to wait.
 * @return True if waiting, false otherwise.
 */
bool Vehicle::shouldWaitAt(double stopPos, double waitDuration) {
    REQUIRE(waitDuration >= 0, "Wait duration must be non-negative");

    static std::unordered_map<double, double> waitTimers;
    double distance = std::fabs(this->getPosition() - stopPos);
    if (distance < 0.5) {
        if (waitTimers[stopPos] < waitDuration) {
            waitTimers[stopPos] += 0.0166;  // Increment timer (e.g. frame time)
            this->speed = 0;
            return true;
        }
    } else {
        waitTimers[stopPos] = 0;
    }
    return false;
}

/**
 * @brief Returns the vehicle type string.
 * @return Type string.
 */
const std::string& Vehicle::getType() const {
    ENSURE(!type.empty(), "Vehicle type should not be empty");
    return type;
}

/**
 * @brief Returns the current position.
 * @return Position on road.
 */
double Vehicle::getPosition() const {
    ENSURE(position >= 0, "Position should be non-negative");
    return position;
}

/**
 * @brief Returns the current speed.
 * @return Speed value.
 */
double Vehicle::getSpeed() const {
    ENSURE(speed >= 0, "Speed should be non-negative");
    ENSURE(speed <= vmax, "Speed should not exceed maximum speed");
    return speed;
}

/**
 * @brief Returns current acceleration.
 * @return Acceleration value.
 */
double Vehicle::getAcceleration() const {
    return acceleration;
}

/**
 * @brief Returns the road pointer.
 * @return Pointer to Road.
 */
const Road* Vehicle::getRoad() const {
    ENSURE(road != nullptr, "Road should not be null");
    return road;
}

/**
 * @brief Sets the vehicle's road.
 * @param r Pointer to new road.
 */
void Vehicle::setRoad(Road* r) {
    REQUIRE(r != nullptr, "Road cannot be null");
    road = r;
    ENSURE(road == r, "Road was not set properly");
}

/**
 * @brief Sets the vehicle's position.
 * @param newPosition New position.
 */
void Vehicle::setPosition(double newPosition) {
    REQUIRE(newPosition >= 0, "Position must be non-negative");
    position = newPosition;
    ENSURE(position == newPosition, "Position was not set properly");
}

/**
 * @brief Sets the vehicle's speed.
 * @param newSpeed New speed value.
 */
void Vehicle::setSpeed(double newSpeed) {
    REQUIRE(newSpeed >= 0, "Speed must be non-negative");
    REQUIRE(newSpeed <= vmax, "Speed cannot exceed maximum speed");
    speed = newSpeed;
    ENSURE(speed == newSpeed, "Speed was not set properly");
}
