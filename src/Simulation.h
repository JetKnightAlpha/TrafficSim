#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <string>

class Road;
class Vehicle;
class TrafficLight;
class VehicleGenerator;

class Simulation {
public:
    // Constructor
    Simulation();

    // Load simulation data from a file
    void loadFromFile(const std::string& filename);

    // Run one simulation step
    void runStep();

    // Run the simulation for a set number of steps
    void run();

    // Output the current state of the simulation
    void outputState() const;

    // Destructor
    ~Simulation();

    // Getters for roads, vehicles, and traffic lights
    const std::vector<Road*>& getRoads() const;
    const std::vector<Vehicle*>& getVehicles() const;
    const std::vector<TrafficLight*>& getTrafficLights() const;

    // Methods to add roads, vehicles, and traffic lights
    void addRoad(Road* road);
    void addVehicle(Vehicle* vehicle);
    void addTrafficLight(TrafficLight* light);

private:
    double currentTime;          // Current simulation time
    int stepCounter;             // Counter for simulation steps
    int vehicleCounter;          // Unique vehicle counter (for IDs)
    std::vector<Road*> roads;    // List of roads
    std::vector<Vehicle*> vehicles;    // List of vehicles
    std::vector<TrafficLight*> trafficLights; // List of traffic lights
    std::vector<VehicleGenerator*> generators; // Vehicle generators
};

#endif // SIMULATION_H
