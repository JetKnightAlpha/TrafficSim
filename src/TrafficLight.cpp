#include "TrafficLight.h"

TrafficLight::TrafficLight(const std::string &roadName, double position, double cycle)
    : fRoadName(roadName),
      fPosition(position),
      fCycle(cycle),
      fTimeSinceChange(0.0),
      fGreen(true)
{
}

TrafficLight::State TrafficLight::getState() const {
    return fGreen ? State::Green : State::Red;
}

bool TrafficLight::isGreen() const {
    return fGreen;
}

void TrafficLight::update(double deltaT) {
    fTimeSinceChange += deltaT;
    if (fTimeSinceChange >= fCycle) {
        fGreen = !fGreen;
        fTimeSinceChange = 0.0;
    }
}

std::string TrafficLight::getRoadName() const {
    return fRoadName;
}

double TrafficLight::getPosition() const {
    return fPosition;
}
