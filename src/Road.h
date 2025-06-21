#ifndef ROAD_H
#define ROAD_H

#include <string>
#include <vector>

class Vehicle;
class TrafficLight;
class Intersection;
class BusStop;
class VehicleGenerator;

/**
 * @class Road
 * @brief Represents a road in the traffic simulation.
 * 
 * Stores vehicles, traffic lights, bus stops, intersections, and connected roads.
 */
class Road {
public:
    /**
     * @brief Constructs a Road with a name and length.
     * 
     * Length is set to minimum 100 if smaller.
     * 
     * @param name Name of the road.
     * @param length Length of the road.
     */
    Road(const std::string& name, int length);

    /**
     * @brief Gets the name of the road.
     * @return const std::string& Name of the road.
     */
    const std::string& getName() const;

    /**
     * @brief Gets the length of the road.
     * @return int Length of the road.
     */
    int getLength() const;

    /**
     * @brief Adds a vehicle to the road.
     * @param vehicle Pointer to the vehicle.
     */
    void addVehicle(Vehicle* vehicle);

    /**
     * @brief Adds a traffic light to the road.
     * @param light Pointer to the traffic light.
     */
    void addTrafficLight(TrafficLight* light);

    /**
     * @brief Adds a connected road.
     * @param road Pointer to the connected road.
     */
    void addRoad(Road* road);

    /**
     * @brief Adds a bus stop to the road.
     * @param stop Pointer to the bus stop.
     */
    void addBusStop(BusStop* stop);

    /**
     * @brief Adds an intersection to the road.
     * @param intersection Pointer to the intersection.
     */
    void addIntersection(Intersection* intersection);

    /**
     * @brief Updates all vehicles and road state for a simulation step.
     */
    void update();

    /**
     * @brief Gets the vehicles on the road.
     * @return const std::vector<Vehicle*>& Vector of vehicle pointers.
     */
    const std::vector<Vehicle*>& getVehicles() const;

    /**
     * @brief Gets the traffic lights on the road.
     * @return const std::vector<TrafficLight*>& Vector of traffic light pointers.
     */
    const std::vector<TrafficLight*>& getTrafficLights() const;

    /**
     * @brief Gets connected roads.
     * @return const std::vector<Road*>& Vector of connected road pointers.
     */
    const std::vector<Road*>& getRoads() const;

    /**
     * @brief Checks if there is a vehicle ahead of the specified vehicle.
     * @param vehicle Pointer to the vehicle.
     * @return true if a leading vehicle exists, false otherwise.
     */
    bool hasLeadingVehicle(const Vehicle* vehicle) const;

    /**
     * @brief Finds the closest vehicle ahead of the specified vehicle.
     * @param vehicle Pointer to the vehicle.
     * @return Vehicle* Pointer to the closest leading vehicle or nullptr if none.
     */
    Vehicle* getLeadingVehicle(const Vehicle* vehicle) const;

    /**
     * @brief Removes a vehicle from the road.
     * @param vehicle Pointer to the vehicle to remove.
     */
    void removeVehicle(Vehicle* vehicle);

    /**
     * @brief Finds a road by name in a list of roads.
     * @param roadName Name of the road.
     * @param roads Vector of road pointers to search.
     * @return Road* Pointer to the road if found, else nullptr.
     */
    static Road* getRoadByName(const std::string& roadName, const std::vector<Road*>& roads);

private:
    std::string name;
    int length;
    std::vector<Vehicle*> vehicles;
    std::vector<TrafficLight*> lights;
    std::vector<Road*> roads;
    std::vector<BusStop*> busStops;
    std::vector<Intersection*> intersections;
};

#endif // ROAD_H
