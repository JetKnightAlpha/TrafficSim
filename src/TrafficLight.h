#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <string>

class Road;

/**
 * @class TrafficLight
 * @brief Represents a traffic light on a road with a green/red cycle.
 * 
 * The traffic light switches state (green/red) cyclically based on a fixed cycle time.
 * It is positioned on a specific road at a given location.
 */
class TrafficLight {
public:
    /**
     * @brief Constructs a TrafficLight at a given position on a road with a cycle duration.
     * @param road Pointer to the Road (must not be nullptr).
     * @param position Position on the road (must be >= 0).
     * @param cycle Cycle time for switching between green and red (must be > 0).
     * @pre road != nullptr
     * @pre position >= 0
     * @pre cycle > 0
     * @post traffic light initialized to green with lastSwitchTime = 0
     */
    TrafficLight(Road* road, double position, int cycle);

    /**
     * @brief Updates the light state depending on elapsed time.
     * Switches state if cycle time has elapsed since last switch.
     * @param time Current simulation time (monotonically increasing).
     * @pre time >= lastSwitchTime
     * @post toggles green/red if cycle time elapsed since lastSwitchTime
     */
    void update(double time);

    /** @return "green" if the light is green, otherwise "red". */
    std::string getStatus() const;

    /** @return true if the light is green, false otherwise. */
    bool isGreen() const;

    /** @return The position of the traffic light on its road (>= 0). */
    double getPosition() const;

private:
    Road* road;
    double position;
    int cycle;
    bool green;
    double lastSwitchTime;
};

#endif // TRAFFICLIGHT_H
