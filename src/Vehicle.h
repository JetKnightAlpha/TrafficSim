#ifndef INC_VEHICLE_H
#define INC_VEHICLE_H

#include <string>

class Vehicle {
public:
    // Constructors
    explicit Vehicle(double speed);
    Vehicle(const std::string &roadName, double position);

    void setPosition(double position);
    double getAcceleration() const;
    double getPosition() const;
    double getSpeed() const;
    std::string getRoadName() const;

    // Updates
    void update(double deltaT);
    void updateAcceleration(const Vehicle* vehicleAhead);

    // Stop/Vertraag-API
    void stopImmediately();
    void applyTemporarySpeedFactor(double factor);
    void resetMaxSpeed();

private:
    std::string fRoadName;
    double fPosition;
    double fSpeed;
    double fAcceleration;

    // Standaardwaarden
    const double kVehicleLength = 4.0;
    const double kVmax         = 16.6;
    const double kAmax         = 1.44;
    const double kBmax         = 4.61;
    const double kFmin         = 4.0;

    // Dynamische max-snelheid (kan verlaagd worden)
    double fCurrentVmax;
};

#endif // INC_VEHICLE_H
