#include "VehicleGenerator.h"
#include "Road.h"
#include "Vehicle.h"

VehicleGenerator::VehicleGenerator(Road* road, int frequency)
    : road(road), frequency(frequency), lastGenerated(-frequency) {}

void VehicleGenerator::update(double currentTime) {
    if (currentTime - lastGenerated >= frequency) {
        road->addVehicle(new Vehicle(road, 0));
        lastGenerated = currentTime;
    }
}