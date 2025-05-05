#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <string>

class Road;

class TrafficLight {
public:
    TrafficLight(Road* road, double position, int cycle);

    void update(double time);
    std::string getStatus() const;
    bool isGreen() const;

    double getPosition() const;
    Road* getRoad() const;

private:
    Road* road;
    double position;
    int cycle;
    bool green;
    double lastSwitchTime;
};

#endif // TRAFFICLIGHT_H
