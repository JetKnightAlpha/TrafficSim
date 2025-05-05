#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Road.h"
#include "Vehicle.h"

class Intersection {
public:
    Intersection(Road* road1, double pos1, Road* road2, double pos2);

    void handleRoadSwitch(Vehicle* vehicle);

private:
    struct RoadConnection {
        Road* road;
        double position;
    };

    std::pair<RoadConnection, RoadConnection> roads;
};

#endif
