#ifndef VEHICLE_H
#define VEHICLE_H

class Road;  // Forward declaration to avoid circular dependencies

class Vehicle {
public:
    // Constructor
    Vehicle(Road* road, double position);

    // Update vehicle state based on acceleration and time step
    void update(double deltaTime);

    // Calculate the vehicle's acceleration based on following distance, speed, and max acceleration
    void calculateAcceleration();

    // Apply traffic light rules (braking near a red light)
    void applyTrafficLightRules();

    // Get current position of the vehicle
    double getPosition() const;

    // Get current speed of the vehicle
    double getSpeed() const;

    double getAcceleration() const { return acceleration; }

private:
    Road* road;          // Pointer to the road the vehicle is on
    double position;     // Current position of the vehicle
    double speed;        // Current speed of the vehicle
    double acceleration; // Current acceleration of the vehicle
    double vmax;         // Max speed the vehicle can reach

    // Define constants for the vehicle simulation
    static const double l;      // Vehicle length (m)
    static const double Vmax;   // Maximum speed (m/s)
    static const double amax;   // Maximum acceleration (m/s²)
    static const double bmax;   // Maximum braking factor (m/s²)
    static const double F_MIN;   // Minimum following distance (m)
    static const double dt;     // Time step (s)
    static const double xs;     // Deceleration distance to traffic light (m)
    static const double xs0;    // Stop distance from traffic light (m)
    static const double s;      // Deceleration factor for stopping
};

#endif // VEHICLE_H
