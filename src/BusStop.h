#ifndef BUSSTOP_H
#define BUSSTOP_H

#include <string>

/**
 * @class BusStop
 * @brief Represents a bus stop located on a specific road, with a defined position and wait time.
 * 
 * A BusStop holds the name of the road it is on, its position along the road, and
 * how long a bus typically waits at this stop.
 */
class BusStop {
public:
    /**
     * @brief Constructs a BusStop with the given road name, position, and wait time.
     * 
     * @param roadName The name of the road the bus stop is on.
     * @param position The position along the road (must be non-negative).
     * @param waitTime The wait time in seconds for buses at this stop (must be non-negative).
     */
    BusStop(const std::string& roadName, double position, double waitTime);

    /**
     * @brief Gets the name of the road where the bus stop is located.
     * 
     * @return const std::string& The road name.
     */
    const std::string& getRoadName() const;

    /**
     * @brief Gets the position of the bus stop on the road.
     * 
     * @return double The position (non-negative).
     */
    double getPosition() const;

    /**
     * @brief Gets the wait time for a bus at this stop.
     * 
     * @return double The wait time in seconds (non-negative).
     */
    double getWaitTime() const;

private:
    std::string roadName;     ///< The name of the road where the bus stop is located.
    double position;          ///< The position of the bus stop along the road.
    double waitTimeSeconds;   ///< The number of seconds a bus waits at this stop.
};

#endif
