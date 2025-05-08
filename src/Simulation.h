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

    void runStep();
    void run();
    void outputState() const;
    void addRoad(Road* road);
    void addTrafficLight(TrafficLight* light);
    void addVehicle(Vehicle* vehicle);
    void addGenerator(VehicleGenerator* generator);
    void addBusStop(BusStop* stop);
    void addIntersection(Intersection* intersection);

    const std::vector<Road*>& getRoads() const;
    const std::vector<Vehicle*>& getVehicles() const;
    const std::vector<TrafficLight*>& getTrafficLights() const;
    const std::vector<Intersection*>& getIntersections() const;
    const std::vector<BusStop*>& getBusStops() const;

    double currentTime;


private:
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
