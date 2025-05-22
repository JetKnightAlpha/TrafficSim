#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
class Road;
class BusStop;

class Vehicle {
public:
    Vehicle(Road* road, double position);

    const Road* getRoad() const;
    double getPosition() const;
    double getSpeed() const;
    double getAcceleration() const;

    virtual const std::string& getType() const;

    void calculateAcceleration();
    void update(double deltaTime);
    void applyTrafficLightRules();
    void setRoad(Road* r);
    void setPosition(double newPosition);
    void setSpeed(double newSpeed);

    bool shouldWaitAt(double stopPos, double waitDuration);

protected:
    std::string type;

private:
    BusStop* bus;
    Road* road;
    double position;
    double speed;
    double acceleration;
    const double vmax;
};

class Auto : public Vehicle {
public:
    Auto(Road* road, double position);
};

class Bus : public Vehicle {
public:
    Bus(Road* road, double position);
};

class Combi : public Vehicle {
public:
    Combi(Road* road, double position);
};

class Ziek : public Vehicle {
public:
    Ziek(Road* road, double position);
};

class Brand : public Vehicle {
public:
    Brand(Road* road, double position);
};

#endif
