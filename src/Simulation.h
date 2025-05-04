#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <string>

class Road;
class Vehicle;
class TrafficLight;
class VehicleGenerator;
class BusStop;
class Intersection;

class Simulation {
public:
    Simulation();

    void loadFromFile(const std::string& filename);
    void runStep();
    void run();
    void outputState() const;

    ~Simulation();

    const std::vector<Road*>& getRoads() const;
    const std::vector<Vehicle*>& getVehicles() const;
    const std::vector<TrafficLight*>& getTrafficLights() const;

    void addRoad(Road* road);
    void addVehicle(Vehicle* vehicle);
    void addTrafficLight(TrafficLight* light);

private:
    double currentTime;
    int stepCounter;
    int vehicleCounter;
    std::vector<Road*> roads;
    std::vector<Vehicle*> vehicles;
    std::vector<TrafficLight*> trafficLights;
    std::vector<VehicleGenerator*> generators;
    std::vector<BusStop*> busStops;
    std::vector<Intersection*> intersections;

};

#endif // SIMULATION_H
