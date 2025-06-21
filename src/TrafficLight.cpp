#include "TrafficLight.h"
#include "DesignByContract.h"

/**
 * @brief Constructs a TrafficLight object.
 * 
 * Initializes a traffic light on a given road at a specific position with a given green/red cycle time.
 * Ensures all parameters are valid and initializes the traffic light to green with lastSwitchTime = 0.
 * 
 * @param road Pointer to the Road (must not be nullptr).
 * @param position Position on the road (must be >= 0).
 * @param cycle Duration of one green/red cycle (must be > 0).
 */
TrafficLight::TrafficLight(Road* road, double position, int cycle)
    : road(road), position(position), cycle(cycle), green(true), lastSwitchTime(0) 
{
    REQUIRE(road != nullptr, "road must not be null");
    REQUIRE(position >= 0.0, "position must be non-negative");
    REQUIRE(cycle > 0, "cycle time must be positive");
    
    ENSURE(this->road == road, "road must be properly set");
    ENSURE(this->position == position, "position must be properly set");
    ENSURE(this->cycle == cycle, "cycle must be properly set");
    ENSURE(this->green == true, "traffic light must start green");
    ENSURE(this->lastSwitchTime == 0, "last switch time must start at 0");
}

/**
 * @brief Updates the traffic light state based on the current simulation time.
 * 
 * Toggles the traffic light between green and red if the cycle duration has elapsed since the last switch.
 * Ensures time does not go backwards and that state changes are consistent with cycle timing.
 * 
 * @param time The current simulation time (must be >= lastSwitchTime).
 */
void TrafficLight::update(double time) {
    REQUIRE(time >= 0.0, "time must be non-negative");
    REQUIRE(time >= lastSwitchTime, "time must not go backwards");
    
    bool oldGreen = green;
    double oldLastSwitchTime = lastSwitchTime;
    
    if (time - lastSwitchTime >= cycle) {
        green = !green;
        lastSwitchTime = time;
        
        ENSURE(green != oldGreen, "light state must have changed");
        ENSURE(lastSwitchTime == time, "last switch time must be updated to current time");
    } else {
        ENSURE(green == oldGreen, "light state must not change if cycle not complete");
        ENSURE(lastSwitchTime == oldLastSwitchTime, "last switch time must not change if cycle not complete");
    }
}

/**
 * @brief Returns whether the traffic light is currently green.
 * @return true if green, false otherwise.
 */
bool TrafficLight::isGreen() const {
    return green;
}

/**
 * @brief Returns the position of the traffic light on the road.
 * @return The non-negative position value.
 */
double TrafficLight::getPosition() const {
    ENSURE(position >= 0.0, "returned position must be non-negative");
    return position;
}

/**
 * @brief Returns the current status of the traffic light as a string.
 * @return "green" if the light is green, "red" if it is red.
 */
std::string TrafficLight::getStatus() const {
    std::string result = green ? "green" : "red";
    
    ENSURE(result == "green" || result == "red", "status must be either 'green' or 'red'");
    ENSURE((result == "green") == green, "status string must match green boolean state");
    
    return result;
}
