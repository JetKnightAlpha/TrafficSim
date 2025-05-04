#ifndef VEHICLEGENERATOR_H
#define VEHICLEGENERATOR_H

#include <string>
class Road;

class VehicleGenerator {
public:
    VehicleGenerator(Road* road, int frequency, const std::string& vehicleType);

    void update(double currentTime);

private:
    Road* road;
    int frequency;
    double lastGenerated;
    std::string type;
};

#endif
