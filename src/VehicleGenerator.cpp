#include "VehicleGenerator.h"
#include "Road.h"
#include "Vehicle.h"

// Constructor initializes vehicle generator with road, frequency, and vehicle type
VehicleGenerator::VehicleGenerator(Road* road, int frequency, const std::string& vehicleType)
    : road(road), frequency(frequency), lastGenerated(0), type(vehicleType) {}

// Updates the generator, creating new vehicles based on frequency
void VehicleGenerator::update(double currentTime) {
    if (currentTime - lastGenerated >= frequency) {
        bool canGenerate = true;
        double vehicleLength = 4.0;

        for (auto* vehicle : road->getVehicles()) {
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
            road->addVehicle(v);
            lastGenerated = currentTime;
        }
    }
}
