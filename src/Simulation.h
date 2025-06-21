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

/**
 * @class Simulation
 * @brief Simulates the traffic system by managing roads, vehicles, traffic lights, and related entities.
 * 
 * Handles the simulation time, step updates, and contains methods to add and access simulation components.
 */
class Simulation {
public:
    /**
     * @brief Constructor initializes the simulation state.
     */
    Simulation();

    /**
     * @brief Executes one step of the simulation.
     * Updates all roads, vehicles, traffic lights, and generators.
     */
    void runStep();

    /**
     * @brief Runs the simulation until all vehicles have left the roads.
     */
    void run();

    /**
     * @brief Outputs the current state of the simulation.
     */
    void outputState() const;

    /**
     * @brief Adds a road to the simulation.
     * @param road Pointer to the road to add.
     */
    void addRoad(Road* road);

    /**
     * @brief Adds a traffic light to the simulation.
     * @param light Pointer to the traffic light to add.
     */
    void addTrafficLight(TrafficLight* light);

    /**
     * @brief Adds a vehicle manually to the simulation (useful for testing).
     * @param vehicle Pointer to the vehicle to add.
     */
    void addVehicle(Vehicle* vehicle);

    /**
     * @brief Adds a vehicle generator to the simulation.
     * @param generator Pointer to the vehicle generator to add.
     */
    void addGenerator(VehicleGenerator* generator);

    /**
     * @brief Adds a bus stop to the simulation.
     * @param stop Pointer to the bus stop to add.
     */
    void addBusStop(BusStop* stop);

    /**
     * @brief Adds an intersection to the simulation.
     * @param intersection Pointer to the intersection to add.
     */
    void addIntersection(Intersection* intersection);

    /**
     * @brief Returns the list of roads in the simulation.
     * @return Vector of pointers to Road objects.
     */
    const std::vector<Road*>& getRoads() const;

    /**
     * @brief Returns the list of vehicles in the simulation.
     * @return Vector of pointers to Vehicle objects.
     */
    const std::vector<Vehicle*>& getVehicles() const;

    /**
     * @brief Returns the list of traffic lights in the simulation.
     * @return Vector of pointers to TrafficLight objects.
     */
    const std::vector<TrafficLight*>& getTrafficLights() const;

    /**
     * @brief Returns the list of intersections in the simulation.
     * @return Vector of pointers to Intersection objects.
     */
    const std::vector<Intersection*>& getIntersections() const;

    /**
     * @brief Returns the list of bus stops in the simulation.
     * @return Vector of pointers to BusStop objects.
     */
    const std::vector<BusStop*>& getBusStops() const;

    /// Current simulation time in seconds
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

#endif // SIMULATION_H
