#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
class Road;
class BusStop;

class Vehicle {
public:
    Vehicle(Road* road, double position, const std::string& type);

    const Road* getRoad() const {
        return road;
    }
    const std::string& getType() const;
    double getPosition() const;
    double getSpeed() const;
    double getAcceleration() const;

    void calculateAcceleration();
    void update(double deltaTime);
    void applyTrafficLightRules();
    void setRoad(Road* r);
    void setPosition(double newPosition);
    void setSpeed(double newSpeed);

    bool shouldWaitAt(double stopPos, double waitDuration);

private:
    BusStop * bus;
    Road* road;
    double position;
    double speed;
    double acceleration;
    const double vmax;
    std::string type;
};

#endif
