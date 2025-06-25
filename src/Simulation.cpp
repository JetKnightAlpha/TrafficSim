#include "Simulation.h"
#include "Parser.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"
#include "BusStop.h"
#include "Intersection.h"
#include "DesignByContract.h"
#include <iostream>
#include <cmath>

/**
 * @brief Constructor initializes the simulation state.
 * Sets current time, step counter, and vehicle counter to initial values.
 */
Simulation::Simulation()
    : currentTime(0), stepCounter(0), vehicleCounter(1) {
    ENSURE(currentTime == 0, "Current time should be initialized to 0");
    ENSURE(stepCounter == 0, "Step counter should be initialized to 0");
    ENSURE(vehicleCounter == 1, "Vehicle counter should be initialized to 1");
}

/**
 * @brief Runs one simulation step.
 * Updates all roads, traffic lights, and vehicle generators.
 * Advances simulation time and increments step counter.
 */
void Simulation::runStep() {
    double oldTime = currentTime;
    int oldStepCounter = stepCounter;

    // Update each road's vehicles and states
    for (auto* road : roads) {
        road->update();

        // Update traffic lights on the road
        for (auto* light : road->getTrafficLights()) {
            light->update(currentTime);
        }
    }

    // Update all vehicle generators
    for (auto* generator : generators) {
        generator->update(currentTime);
    }

    stepCounter++;
    currentTime += 0.0166;  // Approximate timestep (~60 FPS)

    ENSURE(stepCounter == oldStepCounter + 1, "Step counter should be incremented");
    ENSURE(currentTime > oldTime, "Current time should be increased");
}

/**
 * @brief Runs the entire simulation until all vehicles have left the roads.
 * Stops when no vehicles remain.
 * Outputs simulation state at each step.
 */
void Simulation::run() {
    while (true) {
        runStep();

        // Check if all roads have no vehicles
        bool allRoadsEmpty = true;
        for (auto* road : roads) {
            if (!road->getVehicles().empty()) {
                allRoadsEmpty = false;
                break;
            }
        }

        if (allRoadsEmpty) {
            std::cout << "Simulation ended, no vehicles on roads" << std::endl;
            // output.simulationEnded();
            break;
        }

        outputState();
    }
}

/**
 * @brief Outputs the current state of the simulation to the console.
 * Shows step count, current time, and details of vehicles and traffic lights on each road.
 */
void Simulation::outputState() const {
    std::cout << "Increment: " << stepCounter << std::endl;
    std::cout << "Tijd: " << currentTime << std::endl << "\n";

    for (const Road* road : roads) {
        int vehicleCounter = 1;

        // Only print road info if vehicles are present
        if (!road->getVehicles().empty())
            std::cout << "Baan: " << road->getName() << "\n" << std::endl;

        // Print details of each vehicle
        for (const Vehicle* vehicle : road->getVehicles()) {
            int roundedPosition = static_cast<int>(std::round(vehicle->getPosition()));
            double roundedSpeed = std::round(vehicle->getSpeed() * 10.0) / 10.0;

            std::cout << "Voertuig " << vehicleCounter
                      << std::endl
                      << "-> type: " << vehicle->getType()
                      << std::endl
                      << "-> positie: " << roundedPosition
                      << std::endl
                      << "-> snelheid: " << roundedSpeed
                      << "\n" << std::endl;
            // output.printVehicle(vehicle);
            vehicleCounter++;
        }

        // Print traffic light states
        for (const TrafficLight* light : road->getTrafficLights()) {
            if (!road->getVehicles().empty()) {
                std::cout << "Verkeerslicht op positie "
                          << light->getPosition()
                          << " is "
                          << (light->isGreen() ? "groen" : "rood")
                          << "\n" << std::endl;
            }
        }
    }
    std::cout << "-----------------------------------" << std::endl;
}

/**
 * @brief Adds a road to the simulation.
 * @param road Pointer to the road to add (must not be nullptr).
 */
void Simulation::addRoad(Road* road) {
    REQUIRE(road != nullptr, "Road cannot be null");
    
    size_t oldSize = roads.size();
    roads.push_back(road);
    
    ENSURE(roads.size() == oldSize + 1, "Road was not added properly");
}

/**
 * @brief Adds a traffic light to the simulation.
 * @param light Pointer to the traffic light to add (must not be nullptr).
 */
void Simulation::addTrafficLight(TrafficLight* light) {
    REQUIRE(light != nullptr, "Traffic light cannot be null");
    
    size_t oldSize = trafficLights.size();
    trafficLights.push_back(light);
    
    ENSURE(trafficLights.size() == oldSize + 1, "Traffic light was not added properly");
}

/**
 * @brief Adds a vehicle to the simulation manually.
 * Useful for testing purposes.
 * @param vehicle Pointer to the vehicle to add (must not be nullptr).
 */
void Simulation::addVehicle(Vehicle* vehicle) {
    REQUIRE(vehicle != nullptr, "Vehicle cannot be null");
    
    size_t oldSize = vehicles.size();
    vehicles.push_back(vehicle);
    
    ENSURE(vehicles.size() == oldSize + 1, "Vehicle was not added properly");
}

/**
 * @brief Returns a constant reference to the vector of roads.
 * @return Vector of Road pointers.
 */
const std::vector<Road*>& Simulation::getRoads() const {
    return roads;
}

/**
 * @brief Adds a vehicle generator to the simulation.
 * @param generator Pointer to the generator to add (must not be nullptr).
 */
void Simulation::addGenerator(VehicleGenerator* generator) {
    REQUIRE(generator != nullptr, "Generator cannot be null");
    
    size_t oldSize = generators.size();
    generators.push_back(generator);
    
    ENSURE(generators.size() == oldSize + 1, "Generator was not added properly");
}

/**
 * @brief Adds a bus stop to the simulation.
 * @param stop Pointer to the bus stop to add (must not be nullptr).
 */
void Simulation::addBusStop(BusStop* stop) {
    REQUIRE(stop != nullptr, "Bus stop cannot be null");
    
    size_t oldSize = busStops.size();
    busStops.push_back(stop);
    
    ENSURE(busStops.size() == oldSize + 1, "Bus stop was not added properly");
}

/**
 * @brief Adds an intersection to the simulation.
 * @param intersection Pointer to the intersection to add (must not be nullptr).
 */
void Simulation::addIntersection(Intersection* intersection) {
    REQUIRE(intersection != nullptr, "Intersection cannot be null");
    
    size_t oldSize = intersections.size();
    intersections.push_back(intersection);
    
    ENSURE(intersections.size() == oldSize + 1, "Intersection was not added properly");
}

/**
 * @brief Returns a constant reference to the vector of vehicles.
 * @return Vector of Vehicle pointers.
 */
const std::vector<Vehicle*>& Simulation::getVehicles() const {
    return vehicles;
}

/**
 * @brief Returns a constant reference to the vector of traffic lights.
 * @return Vector of TrafficLight pointers.
 */
const std::vector<TrafficLight*>& Simulation::getTrafficLights() const {
    return trafficLights;
}

/**
 * @brief Returns a constant reference to the vector of intersections.
 * @return Vector of Intersection pointers.
 */
const std::vector<Intersection*>& Simulation::getIntersections() const {
    return intersections;
}

/**
 * @brief Returns a constant reference to the vector of bus stops.
 * @return Vector of BusStop pointers.
 */
const std::vector<BusStop*>& Simulation::getBusStops() const {
    return busStops;
}
