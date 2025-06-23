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
     * @post currentTime == 0
     * @post stepCounter == 0
     * @post vehicleCounter == 0
     */
    Simulation();

    /**
     * @brief Executes one step of the simulation.
     * Updates all roads, vehicles, traffic lights, and generators.
     * @post simulation state advanced by one step
     */
    void runStep();

    /**
     * @brief Runs the simulation until all vehicles have left the roads.
     * @post simulation ends when no vehicles remain on roads
     */
    void run();

    /**
     * @brief Outputs the current state of the simulation.
     * @post simulation state printed or logged to output
     */
    void outputState() const;

    /**
     * @brief Adds a road to the simulation.
     * @param road Pointer to the road to add.
     * @pre road != nullptr
     * @post road is included in getRoads()
     */
    void addRoad(Road* road);

    /**
     * @brief Adds a traffic light to the simulation.
     * @param light Pointer to the traffic light to add.
     * @pre light != nullptr
     * @post light is included in getTrafficLights()
     */
    void addTrafficLight(TrafficLight* light);

    /**
     * @brief Adds a vehicle manually to the simulation (useful for testing).
     * @param vehicle Pointer to the vehicle to add.
     * @pre vehicle != nullptr
     * @post vehicle is included in getVehicles()
     */
    void addVehicle(Vehicle* vehicle);

    /**
     * @brief Adds a vehicle generator to the simulation.
     * @param generator Pointer to the vehicle generator to add.
     * @pre generator != nullptr
     * @post generator is included in generators list
     */
    void addGenerator(VehicleGenerator* generator);

    /**
     * @brief Adds a bus stop to the simulation.
     * @param stop Pointer to the bus stop to add.
     * @pre stop != nullptr
     * @post stop is included in getBusStops()
     */
    void addBusStop(BusStop* stop);

    /**
     * @brief Adds an intersection to the simulation.
     * @param intersection Pointer to the intersection to add.
     * @pre intersection != nullptr
     * @post intersection is included in getIntersections()
     */
    void addIntersection(Intersection* intersection);

    /**
     * @brief Returns the list of roads in the simulation.
     * @return Vector of pointers to Road objects.
     * @post returned vector reflects all added roads
     */
    const std::vector<Road*>& getRoads() const;

    /**
     * @brief Returns the list of vehicles in the simulation.
     * @return Vector of pointers to Vehicle objects.
     * @post returned vector reflects all added vehicles
     */
    const std::vector<Vehicle*>& getVehicles() const;

    /**
     * @brief Returns the list of traffic lights in the simulation.
     * @return Vector of pointers to TrafficLight objects.
     * @post returned vector reflects all added traffic lights
     */
    const std::vector<TrafficLight*>& getTrafficLights() const;

    /**
     * @brief Returns the list of intersections in the simulation.
     * @return Vector of pointers to Intersection objects.
     * @post returned vector reflects all added intersections
     */
    const std::vector<Intersection*>& getIntersections() const;

    /**
     * @brief Returns the list of bus stops in the simulation.
     * @return Vector of pointers to BusStop objects.
     * @post returned vector reflects all added bus stops
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
