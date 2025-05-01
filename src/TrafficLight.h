#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <string>

class Road;

class TrafficLight {
public:
    TrafficLight(Road* road, int position, int cycle);

    // Update the light status based on the current time
    void update(double time);

    // Get the status of the light (green or red)
    std::string getStatus() const;

    // Check if the light is green
    bool isGreen() const;

    // Get the position of the light on the road
    int getPosition() const;

private:
    Road* road;            // Road the light belongs to
    int position;          // Position of the traffic light on the road
    int cycle;             // Cycle time for the green/red light switch
    bool green;            // Whether the light is green
    double lastSwitchTime; // Last time the light changed status
};

#endif // TRAFFICLIGHT_H
