#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

class Road;
class BusStop;

/**
 * @class Vehicle
 * @brief Represents a vehicle traveling on a road.
 * 
 * This is the base class for different types of vehicles like Auto, Bus, Combi, etc.
 * It handles position, speed, acceleration, and interaction with traffic elements.
 */
class Vehicle {
public:
    /**
     * @brief Constructs a Vehicle on a specified road at a given position.
     * @param road Pointer to the Road the vehicle is on.
     * @param position Initial position on the road (non-negative).
     * @pre road != nullptr
     * @pre position >= 0
     * @post getRoad() == road
     * @post getPosition() == position
     * @post getSpeed() == 0 (initial speed)
     */
    Vehicle(Road* road, double position);

    /** @brief Returns a pointer to the Road the vehicle is on. */
    const Road* getRoad() const;

    /** @brief Returns the current position of the vehicle. */
    double getPosition() const;

    /** @brief Returns the current speed of the vehicle. */
    double getSpeed() const;

    /** @brief Returns the current acceleration of the vehicle. */
    double getAcceleration() const;

    /** 
     * @brief Returns the type of vehicle as a string.
     * @return Vehicle type (e.g., "auto", "bus").
     * @post returns a non-empty string representing the vehicle type
     */
    virtual const std::string& getType() const;

    /**
     * @brief Calculates the vehicle's acceleration based on road conditions and leading vehicle.
     * @post acceleration updated based on current traffic situation
     */
    void calculateAcceleration();

    /**
     * @brief Updates the vehicle's position and speed using acceleration and elapsed time.
     * @param deltaTime Time step for the update (must be positive).
     * @pre deltaTime > 0
     * @post position and speed updated according to kinematics equations
     */
    void update(double deltaTime);

    /**
     * @brief Applies traffic light rules, possibly slowing down near red lights.
     * @post speed or acceleration may be adjusted to comply with traffic light states
     */
    void applyTrafficLightRules();

    /**
     * @brief Sets the road the vehicle is currently on.
     * @param r Pointer to the new Road (non-null).
     * @pre r != nullptr
     * @post getRoad() == r
     */
    void setRoad(Road* r);

    /**
     * @brief Sets the vehicle's position on the road.
     * @param newPosition New position (must be non-negative).
     * @pre newPosition >= 0
     * @post getPosition() == newPosition
     */
    void setPosition(double newPosition);

    /**
     * @brief Sets the vehicle's speed.
     * @param newSpeed New speed (non-negative and no greater than vmax).
     * @pre newSpeed >= 0
     * @pre newSpeed <= vmax
     * @post getSpeed() == newSpeed
     */
    void setSpeed(double newSpeed);

    /**
     * @brief Determines if the vehicle should wait at a bus stop.
     * @param stopPos Position of the bus stop.
     * @param waitDuration Duration to wait at the stop.
     * @return True if the vehicle is waiting, false otherwise.
     * @pre stopPos >= 0
     * @pre waitDuration >= 0
     */
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

/**
 * @class Auto
 * @brief Represents a standard car.
 */
class Auto : public Vehicle {
public:
    Auto(Road* road, double position);
};

/**
 * @class Bus
 * @brief Represents a bus.
 */
class Bus : public Vehicle {
public:
    Bus(Road* road, double position);
};

/**
 * @class Combi
 * @brief Represents a police car.
 */
class Combi : public Vehicle {
public:
    Combi(Road* road, double position);
};

/**
 * @class Ziek
 * @brief Represents an ambulance.
 */
class Ziek : public Vehicle {
public:
    Ziek(Road* road, double position);
};

/**
 * @class Brand
 * @brief Represents a fire truck.
 */
class Brand : public Vehicle {
public:
    Brand(Road* road, double position);
};

#endif // VEHICLE_H
