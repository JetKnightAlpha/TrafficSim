#ifndef INC_ROAD_H
#define INC_ROAD_H

#include <string>
#include <vector>
#include "Vehicle.h"

class Road {
public:
    Road(const std::string &name, double length);
    Road(double length);

    void addVehicle(const Vehicle& vehicle);
    double getLength() const;
    std::string getName() const;

    // Eventueel: simulateStep
    void simulateStep(double deltaTime);

    const std::vector<Vehicle>& getVehicles() const;

private:
    std::string fName;
    double fLength;
    std::vector<Vehicle> fVehicles;
};

#endif // INC_ROAD_H
