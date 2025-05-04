#ifndef BUSSTOP_H
#define BUSSTOP_H

#include <string>
#include "Vehicle.h"

class BusStop {
public:
    BusStop(const std::string& roadName, double position, double waitTime);

    const std::string& getRoadName() const;
    double getPosition() const;
    double getWaitTime() const;

private:
    std::string roadName;
    double position;
    double waitTimeSeconds;
};

#endif
