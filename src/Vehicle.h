#ifndef VEHICLE_H
#define VEHICLE_H
// test
class Road;

class Vehicle {
public:
    Vehicle(Road* road, double position);

    void update(double deltaTime);

    void calculateAcceleration();

    void applyTrafficLightRules();

    double getPosition() const;

    double getSpeed() const;

    double getAcceleration() const { return acceleration; }

private:
    Road* road;
    double position;
    double speed;
    double acceleration;
    double vmax;

    static const double l;
    static const double Vmax;
    static const double amax;
    static const double bmax;
    static const double F_MIN;
    static const double dt;
    static const double xs;
    static const double xs0;
    static const double s;
};

#endif // VEHICLE_H
