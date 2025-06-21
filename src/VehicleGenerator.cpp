#include "VehicleGenerator.h"
#include "Road.h"
#include "Vehicle.h"
#include "DesignByContract.h"

/**
 * @brief Constructs a VehicleGenerator object.
 * 
 * Initializes the generator with a target road, generation frequency, and vehicle type.
 * Ensures all inputs are valid and that the last generated time starts at zero.
 * 
 * @param road Pointer to the Road to generate vehicles on (must not be nullptr).
 * @param frequency Time interval between vehicle generation events (must be positive).
 * @param vehicleType String representing the type of vehicle to generate (must be valid).
 */
VehicleGenerator::VehicleGenerator(Road* road, int frequency, const std::string& vehicleType)
    : road(road), frequency(frequency), lastGenerated(0), type(vehicleType) 
{
    REQUIRE(road != nullptr, "road must not be null");
    REQUIRE(frequency > 0, "frequency must be positive");
    REQUIRE(!vehicleType.empty(), "vehicleType must not be empty");
    REQUIRE(vehicleType == "auto" || vehicleType == "bus" || vehicleType == "politiecombi" || 
            vehicleType == "ziekenwagen" || vehicleType == "brandweerwagen", 
            "vehicleType must be a valid type");
    
    ENSURE(this->road == road, "road must be properly set");
    ENSURE(this->frequency == frequency, "frequency must be properly set");
    ENSURE(this->type == vehicleType, "vehicle type must be properly set");
    ENSURE(this->lastGenerated == 0, "last generated time must start at 0");
}

/**
 * @brief Updates the vehicle generator state, possibly creating a new vehicle.
 * 
 * Checks if enough time has passed since the last vehicle generation according to frequency.
 * Also verifies that the start of the road is clear to avoid overlapping vehicles.
 * If conditions are met, creates a new vehicle of the specified type at position zero and adds it to the road.
 * 
 * @param currentTime The current simulation time (must be >= lastGenerated).
 */
void VehicleGenerator::update(double currentTime) {
    REQUIRE(currentTime >= 0.0, "currentTime must be non-negative");
    REQUIRE(currentTime >= lastGenerated, "time must not go backwards");
    REQUIRE(road != nullptr, "road must still be valid");
    
    size_t initialVehicleCount = road->getVehicles().size();
    double oldLastGenerated = lastGenerated;
    
    if (currentTime - lastGenerated >= frequency) {
        bool canGenerate = true;
        double vehicleLength = 4.0;

        // Check if the first ~2 vehicle lengths of the road are clear
        for (auto* vehicle : road->getVehicles()) {
            REQUIRE(vehicle != nullptr, "all vehicles in road must be valid");
            double vehiclePosition = vehicle->getPosition();
            if (vehiclePosition > 0 && vehiclePosition < 2 * vehicleLength) {
                canGenerate = false;
                break;
            }
        }
        
        if (canGenerate) {
            Vehicle* v = nullptr;

            if (type == "auto") {
                v = new Auto(road, 0);
            } else if (type == "bus") {
                v = new Bus(road, 0);
            } else if (type == "politiecombi") {
                v = new Combi(road, 0);
            } else if (type == "ziekenwagen") {
                v = new Ziek(road, 0);
            } else if (type == "brandweerwagen") {
                v = new Brand(road, 0);
            }
            
            REQUIRE(v != nullptr, "vehicle creation must succeed for valid types");
            REQUIRE(v->getRoad() == road, "new vehicle must be on the correct road");
            REQUIRE(v->getPosition() == 0, "new vehicle must start at position 0");
            
            road->addVehicle(v);
            lastGenerated = currentTime;
            
            ENSURE(road->getVehicles().size() == initialVehicleCount + 1, 
                   "exactly one vehicle must be added");
            ENSURE(lastGenerated == currentTime, "last generated time must be updated");
        } else {
            ENSURE(road->getVehicles().size() == initialVehicleCount, 
                   "no vehicles should be added when generation blocked");
            ENSURE(lastGenerated == oldLastGenerated, 
                   "last generated time should not change when generation blocked");
        }
    } else {
        ENSURE(road->getVehicles().size() == initialVehicleCount, 
               "no vehicles should be added when frequency not met");
        ENSURE(lastGenerated == oldLastGenerated, 
               "last generated time should not change when frequency not met");
    }
}
