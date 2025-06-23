#ifndef VEHICLEGENERATOR_H
#define VEHICLEGENERATOR_H

#include <string>

class Road;

/**
 * @class VehicleGenerator
 * @brief Periodically generates vehicles of a specified type on a road.
 * 
 * Generates vehicles at fixed time intervals (frequency) while ensuring
 * sufficient spacing at the road's start to prevent overcrowding.
 */
class VehicleGenerator {
public:
    /**
     * @brief Creates a VehicleGenerator on a given road.
     * @param road Pointer to the Road (must not be nullptr).
     * @param frequency Time interval (in simulation time units) between vehicle generations (must be > 0).
     * @param vehicleType Vehicle type string: "auto", "bus", "politiecombi", "ziekenwagen", or "brandweerwagen".
     * @pre road != nullptr
     * @pre frequency > 0
     * @pre vehicleType is one of the accepted types
     * @post lastGenerated initialized (likely to zero or negative to allow immediate generation)
     */
    VehicleGenerator(Road* road, int frequency, const std::string& vehicleType);

    /**
     * @brief Updates the generator state; may spawn a vehicle if conditions allow.
     * @param currentTime Current simulation time (monotonically increasing).
     * @pre currentTime >= lastGenerated
     * @post possibly creates a new vehicle if frequency interval elapsed and road start is free
     */
    void update(double currentTime);

private:
    Road* road;
    int frequency;
    double lastGenerated;
    std::string type;
};

#endif // VEHICLEGENERATOR_H
