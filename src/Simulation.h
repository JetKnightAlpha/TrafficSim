#ifndef INC_SIMULATION_H
#define INC_SIMULATION_H

#include <vector>
#include <memory>
#include <string>
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"

class Simulation {
public:
    Simulation();
    ~Simulation();

    // Inlezen van bestand (Use-cases 1.1 / 1.2)
    bool loadFromFile(const std::string& filename);

    // 1 stap simulatie (handig in tests)
    void runStep();

    // Oude "loadScenario" (optioneel, kan leeg blijven)
    void loadScenario(const std::string &filename);

    // Hoofdloop
    void run();

    // Getters (const)
    const std::vector<std::unique_ptr<Road>>& getRoads() const;
    const std::vector<std::unique_ptr<Vehicle>>& getVehicles() const;
    const std::vector<std::unique_ptr<TrafficLight>>& getTrafficLights() const;

    // Toegevoegde methodes om items toe te voegen
    void addRoad(std::unique_ptr<Road> road);
    void addVehicle(std::unique_ptr<Vehicle> vehicle);
    void addTrafficLight(std::unique_ptr<TrafficLight> light);

private:
    std::vector<std::unique_ptr<Road>> fRoads;
    std::vector<std::unique_ptr<Vehicle>> fVehicles;
    std::vector<std::unique_ptr<TrafficLight>> fTrafficLights;
    std::vector<std::unique_ptr<VehicleGenerator>> fGenerators;

    double fSimTime;
    const double kDeltaT = 0.0166;
    const double kSimDuration = 10.0;

    // Constants uit de behoefte­specificatie
    static constexpr double kStopDistance     = 15.0;
    static constexpr double kSlowDownDistance = 50.0;
    static constexpr double kSlowFactor       = 0.4;

    void update();
    void outputState() const;
    double getRoadLength(const std::string &roadName) const;
};

#endif // INC_SIMULATION_H
