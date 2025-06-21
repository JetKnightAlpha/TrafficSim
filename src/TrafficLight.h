#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <string>

/**
 * @class TrafficLight
 * @brief Represents a traffic light on a road with a green/red cycle.
 * 
 * The traffic light cycles between green and red states based on a fixed cycle time.
 * It is associated with a specific road and a position on that road.
 */
class Road;

class TrafficLight {
public:
    /**
     * @brief Constructs a TrafficLight on a given road at a specific position with a cycle time.
     * @param road Pointer to the Road where the traffic light is located (must not be nullptr).
     * @param position Position of the traffic light on the road (non-negative).
     * @param cycle Duration of the green/red cycle in time units (must be positive).
     */
    TrafficLight(Road* road, double position, int cycle);

    /**
     * @brief Updates the traffic light's state based on the current time.
     * 
     * Switches the light from green to red or vice versa if the cycle time has elapsed.
     * @param time The current simulation time (must not go backwards).
     */
    void update(double time);

    /**
     * @brief Returns the current status of the traffic light as a string.
     * @return "green" if the light is green, "red" otherwise.
     */
    std::string getStatus() const;

    /**
     * @brief Checks if the traffic light is currently green.
     * @return true if green, false if red.
     */
    bool isGreen() const;

    /**
     * @brief Gets the position of the traffic light on the road.
     * @return The non-negative position of the traffic light.
     */
    double getPosition() const;

private:
    Road* road;
    double position;
    int cycle;
    bool green;
    double lastSwitchTime;
};

#endif // TRAFFICLIGHT_H
