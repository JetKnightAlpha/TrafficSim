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
     * @pre road1 != nullptr
     * @pre road2 != nullptr
     * @pre pos1 >= 0.0
     * @pre pos2 >= 0.0
     * @pre road1 != road2
     * @post roads.first.road == road1 && roads.first.position == pos1
     * @post roads.second.road == road2 && roads.second.position == pos2
     * 
     * @param road1 Pointer to the first road.
     * @param pos1 Position on the first road.
     * @param road2 Pointer to the second road.
     * @param pos2 Position on the second road.
     */
    Intersection(Road* road1, double pos1, Road* road2, double pos2);

    /**
     * @brief Handles the logic for switching a vehicle to the connected road.
     * @pre vehicle != nullptr
     * @pre vehicle is on one of the connected roads
     * @post vehicle is either unchanged or moved to the other road with updated position
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
