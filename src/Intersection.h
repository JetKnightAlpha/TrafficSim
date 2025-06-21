#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Road.h"
#include "Vehicle.h"

/**
 * @class Intersection
 * @brief Represents a connection between two roads where vehicles can potentially switch from one to the other.
 * 
 * Each intersection links two distinct roads at specific positions. When a vehicle approaches
 * the intersection, there is a probability that it will switch to the connected road.
 */
class Intersection {
public:
    /**
     * @brief Creates a new intersection between two roads at specified positions.
     * 
     * @param road1 Pointer to the first road.
     * @param pos1 Position on the first road.
     * @param road2 Pointer to the second road.
     * @param pos2 Position on the second road.
     */
    Intersection(Road* road1, double pos1, Road* road2, double pos2);

    /**
     * @brief Handles the logic for switching a vehicle to the connected road.
     * 
     * If the vehicle is near the intersection and the switch condition is met,
     * the vehicle is moved from one road to the other.
     * 
     * @param vehicle The vehicle to potentially switch.
     */
    void handleRoadSwitch(Vehicle* vehicle);

private:
    /**
     * @brief Helper struct representing a road and a position on that road.
     */
    struct RoadConnection {
        Road* road;       ///< Pointer to the road.
        double position;  ///< Position on the road.
    };

    std::pair<RoadConnection, RoadConnection> roads; ///< Pair of connected roads with positions.
};

#endif
