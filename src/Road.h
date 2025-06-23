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
     * Length is set to minimum 100 if smaller.
     * 
     * @param name Name of the road.
     * @param length Length of the road.
     * @pre length >= 0
     * @post getName() == name
     * @post getLength() >= 100
     */
    Road(const std::string& name, int length);

    /**
     * @brief Gets the name of the road.
     * @return const std::string& Name of the road.
     * @post !getName().empty()
     */
    const std::string& getName() const;

    /**
     * @brief Gets the length of the road.
     * @return int Length of the road.
     * @post getLength() >= 100
     */
    int getLength() const;

    /**
     * @brief Adds a vehicle to the road.
     * @param vehicle Pointer to the vehicle.
     * @pre vehicle != nullptr
     * @post getVehicles().size() increased by 1
     */
    void addVehicle(Vehicle* vehicle);

    /**
     * @brief Adds a traffic light to the road.
     * @param light Pointer to the traffic light.
     * @pre light != nullptr
     * @post getTrafficLights().size() increased by 1
     */
    void addTrafficLight(TrafficLight* light);

    /**
     * @brief Adds a connected road.
     * @param road Pointer to the connected road.
     * @pre road != nullptr
     * @post getRoads().size() increased by 1
     */
    void addRoad(Road* road);

    /**
     * @brief Adds a bus stop to the road.
     * @param stop Pointer to the bus stop.
     * @pre stop != nullptr
     * @post busStops size increased by 1
     */
    void addBusStop(BusStop* stop);

    /**
     * @brief Adds an intersection to the road.
     * @param intersection Pointer to the intersection.
     * @pre intersection != nullptr
     * @post intersections size increased by 1
     */
    void addIntersection(Intersection* intersection);

    /**
     * @brief Updates all vehicles and road state for a simulation step.
     * @post state of vehicles and road updated appropriately
     */
    void update();

    /**
     * @brief Gets the vehicles on the road.
     * @return const std::vector<Vehicle*>& Vector of vehicle pointers.
     * @post returned vector is valid and reflects current vehicles
     */
    const std::vector<Vehicle*>& getVehicles() const;

    /**
     * @brief Gets the traffic lights on the road.
     * @return const std::vector<TrafficLight*>& Vector of traffic light pointers.
     * @post returned vector is valid and reflects current lights
     */
    const std::vector<TrafficLight*>& getTrafficLights() const;

    /**
     * @brief Gets connected roads.
     * @return const std::vector<Road*>& Vector of connected road pointers.
     * @post returned vector is valid and reflects connected roads
     */
    const std::vector<Road*>& getRoads() const;

    /**
     * @brief Checks if there is a vehicle ahead of the specified vehicle.
     * @param vehicle Pointer to the vehicle.
     * @return true if a leading vehicle exists, false otherwise.
     * @pre vehicle != nullptr
     */
    bool hasLeadingVehicle(const Vehicle* vehicle) const;

    /**
     * @brief Finds the closest vehicle ahead of the specified vehicle.
     * @param vehicle Pointer to the vehicle.
     * @return Vehicle* Pointer to the closest leading vehicle or nullptr if none.
     * @pre vehicle != nullptr
     */
    Vehicle* getLeadingVehicle(const Vehicle* vehicle) const;

    /**
     * @brief Removes a vehicle from the road.
     * @param vehicle Pointer to the vehicle to remove.
     * @pre vehicle != nullptr
     * @post vehicle is no longer in getVehicles()
     */
    void removeVehicle(Vehicle* vehicle);

    /**
     * @brief Finds a road by name in a list of roads.
     * @param roadName Name of the road.
     * @param roads Vector of road pointers to search.
     * @return Road* Pointer to the road if found, else nullptr.
     * @pre !roadName.empty()
     * @post returned pointer is nullptr or a road with matching name
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
