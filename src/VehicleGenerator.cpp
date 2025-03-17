#include "VehicleGenerator.h"

VehicleGenerator::VehicleGenerator(const std::string &roadName, double frequency)
    : fRoadName(roadName),
      fFrequency(frequency),
      fTimeSinceLast(0.0)
{
}

void VehicleGenerator::update(double deltaT) {
    fTimeSinceLast += deltaT;
}

bool VehicleGenerator::shouldGenerate() const {
    return (fTimeSinceLast >= fFrequency);
}

void VehicleGenerator::resetTimer() {
    fTimeSinceLast = 0.0;
}

std::string VehicleGenerator::getRoadName() const {
    return fRoadName;
}
