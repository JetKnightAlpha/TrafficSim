#ifndef VEHICLEGENERATOR_H
#define VEHICLEGENERATOR_H

#include <string>

class Road;

/**
 * @class VehicleGenerator
 * @brief Generates vehicles of a specified type on a road at a fixed frequency.
 * 
 * VehicleGenerator periodically attempts to create new vehicles on its associated road,
 * respecting spacing constraints to avoid overcrowding at the start of the road.
 */
class VehicleGenerator {
public:
    /**
     * @brief Constructs a VehicleGenerator for a given road.
     * @param road Pointer to the Road where vehicles will be generated (must not be nullptr).
     * @param frequency Time interval between vehicle generations (must be positive).
     * @param vehicleType The type of vehicle to generate. Valid types:
     *        "auto", "bus", "politiecombi", "ziekenwagen", "brandweerwagen".
     */
    VehicleGenerator(Road* road, int frequency, const std::string& vehicleType);

    /**
     * @brief Updates the generator, possibly creating a new vehicle based on the current time.
     * 
     * A new vehicle is generated if enough time has passed since the last generation,
     * and if the road start is not blocked by existing vehicles.
     * @param currentTime The current simulation time (must not go backwards).
     */
    void update(double currentTime);

private:
    Road* road;
    int frequency;
    double lastGenerated;
    std::string type;
};

#endif // VEHICLEGENERATOR_H
