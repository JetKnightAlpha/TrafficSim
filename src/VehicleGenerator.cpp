#include "VehicleGenerator.h"
#include "Road.h"
#include "Vehicle.h"

VehicleGenerator::VehicleGenerator(Road* road, int frequency, const std::string& vehicleType)
    : road(road), frequency(frequency), lastGenerated(-frequency), type(vehicleType) {}

void VehicleGenerator::update(double currentTime) {
    if (currentTime - lastGenerated >= frequency) {
        Vehicle* v = new Vehicle(road, 0, type);
        road->addVehicle(v);
        lastGenerated = currentTime;
    }
}
