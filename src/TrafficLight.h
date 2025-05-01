#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <string>

class Road;

class TrafficLight {
public:
    TrafficLight(Road* road, int position, int cycle);

    void update(double time);

    std::string getStatus() const;

    bool isGreen() const;

    int getPosition() const;

private:
    Road* road;
    int position;
    int cycle;
    bool green;
    double lastSwitchTime;
};

#endif // TRAFFICLIGHT_H
