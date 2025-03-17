#include "Vehicle.h"
#include <cmath>
#include <algorithm>

Vehicle::Vehicle(double speed)
    : fRoadName("test_road"),
      fPosition(0.0),
      fSpeed(speed),
      fAcceleration(0.0),
      fCurrentVmax(kVmax)
{
}

Vehicle::Vehicle(const std::string &roadName, double position)
    : fRoadName(roadName),
      fPosition(position),
      fSpeed(0.0),
      fAcceleration(0.0),
      fCurrentVmax(kVmax)
{
}

void Vehicle::setPosition(double position) {
    fPosition = position;
}

double Vehicle::getAcceleration() const {
    return fAcceleration;
}

double Vehicle::getPosition() const {
    return fPosition;
}

double Vehicle::getSpeed() const {
    return fSpeed;
}

std::string Vehicle::getRoadName() const {
    return fRoadName;
}

void Vehicle::update(double deltaT) {
    double newSpeed = fSpeed + fAcceleration * deltaT;
    if (newSpeed < 0) {
        // Snelheid zou negatief worden => clip naar 0
        if (std::abs(fAcceleration) > 1e-9) {
            fPosition -= (fSpeed * fSpeed) / (2.0 * std::abs(fAcceleration));
        }
        fSpeed = 0.0;
    } else {
        fSpeed = newSpeed;
        fPosition += fSpeed * deltaT + 0.5 * fAcceleration * deltaT * deltaT;
    }
}

void Vehicle::updateAcceleration(const Vehicle* vehicleAhead) {
    if (fCurrentVmax < 1e-9) {
        fCurrentVmax = kVmax;
    }

    if (vehicleAhead) {
        double deltaX = vehicleAhead->getPosition() - fPosition - kVehicleLength;
        double deltaV = fSpeed - vehicleAhead->getSpeed();
        double term   = fSpeed;
        double denominator = std::max(deltaX, 0.1);
        double delta = (kFmin + std::max(0.0, term + (fSpeed*deltaV)/(2.0*std::sqrt(kAmax*kBmax))))
                       / denominator;

        double speedRatio = fSpeed / fCurrentVmax;
        fAcceleration = kAmax * (1 - std::pow(speedRatio, 4) - delta*delta);
    } else {
        // Geen voorligger => 'free road'
        double speedRatio = fSpeed / fCurrentVmax;
        fAcceleration = kAmax * (1 - std::pow(speedRatio, 4));
    }
}

void Vehicle::stopImmediately() {
    fSpeed = 0.0;
    fAcceleration = 0.0;
}

void Vehicle::applyTemporarySpeedFactor(double factor) {
    fCurrentVmax = kVmax * factor;
    if (fSpeed > fCurrentVmax) {
        double diff  = fSpeed - fCurrentVmax;
        double ratio = diff / fSpeed;
        fAcceleration = -std::abs(kBmax) * ratio;
    }
}

void Vehicle::resetMaxSpeed() {
    fCurrentVmax = kVmax;
}
