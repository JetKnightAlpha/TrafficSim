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
    ~Simulation();

    void loadFromFile(const std::string& filename);
    void runStep();
    void run();
    void outputState() const;
    void addRoad(Road* road);
    void addTrafficLight(TrafficLight* light);
    void addVehicle(Vehicle* vehicle);

    const std::vector<Road*>& getRoads() const;
    const std::vector<Vehicle*>& getVehicles() const;
    const std::vector<TrafficLight*>& getTrafficLights() const;
    const std::vector<Intersection*>& getIntersections() const;

    double currentTime;


private:
    void processVehicles();
    void processTrafficLights();
    void processGenerators();

    std::vector<Road*> roads;
    std::vector<Vehicle*> vehicles;
    std::vector<TrafficLight*> trafficLights;
    std::vector<VehicleGenerator*> generators;
    std::vector<BusStop*> busStops;
    std::vector<Intersection*> intersections;

    int stepCounter;
    int vehicleCounter;
};

#endif
