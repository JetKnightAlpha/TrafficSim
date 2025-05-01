#ifndef ROAD_H
#define ROAD_H

#include <string>
#include <vector>
class Vehicle;
class TrafficLight;

class Road {
public:
    Road(const std::string& name, int length);
    const std::string& getName() const;
    int getLength() const;

    void addVehicle(Vehicle* vehicle);
    void addTrafficLight(TrafficLight* light);

    const std::vector<Vehicle*>& getVehicles() const;
    const std::vector<TrafficLight*>& getTrafficLights() const;

    bool hasLeadingVehicle(const Vehicle* vehicle) const;
    Vehicle* getLeadingVehicle(const Vehicle* vehicle) const;

private:
    std::string name;
    int length;
    std::vector<Vehicle*> vehicles;
    std::vector<TrafficLight*> lights;
};

#endif
