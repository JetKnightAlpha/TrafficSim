#ifndef INC_TRAFFICLIGHT_H
#define INC_TRAFFICLIGHT_H

#include <string>

class TrafficLight {
public:
    enum class State { Red, Green };

    TrafficLight(const std::string &roadName, double position, double cycle);

    // Ophalen
    State getState() const;
    bool isGreen() const;

    // Updaten
    void update(double deltaT);

    // Info
    std::string getRoadName() const;
    double getPosition() const;

private:
    std::string fRoadName;
    double fPosition;
    double fCycle;
    double fTimeSinceChange;
    bool fGreen;
};

#endif // INC_TRAFFICLIGHT_H
