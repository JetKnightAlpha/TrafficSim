#include "Simulation.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>
#include <fstream>

Simulation::Simulation() : fSimTime(0.0) {}
Simulation::~Simulation() = default;

// --- addRoad / addVehicle / addTrafficLight ---
void Simulation::addRoad(std::unique_ptr<Road> road) {
    fRoads.push_back(std::move(road));
}
void Simulation::addVehicle(std::unique_ptr<Vehicle> vehicle) {
    fVehicles.push_back(std::move(vehicle));
}
void Simulation::addTrafficLight(std::unique_ptr<TrafficLight> light) {
    fTrafficLights.push_back(std::move(light));
}

// --- getters ---
const std::vector<std::unique_ptr<Road>>& Simulation::getRoads() const {
    return fRoads;
}
const std::vector<std::unique_ptr<Vehicle>>& Simulation::getVehicles() const {
    return fVehicles;
}
const std::vector<std::unique_ptr<TrafficLight>>& Simulation::getTrafficLights() const {
    return fTrafficLights;
}

// --- loadFromFile(...) ---
bool Simulation::loadFromFile(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        std::cerr << "Kon bestand niet openen: " << filename << "\n";
        return false;
    }

    std::string elementType;
    while (ifs >> elementType) {
        if (elementType == "BAAN") {
            std::string roadName;
            double length;
            ifs >> roadName >> length;
            if (length <= 0) {
                std::cerr << "Ongeldige baandlengte: " << length << "\n";
                continue;
            }
            auto road = std::make_unique<Road>(roadName, length);
            fRoads.push_back(std::move(road));
        }
        else if (elementType == "VERKEERSLICHT") {
            std::string roadName;
            double position, cycle;
            ifs >> roadName >> position >> cycle;
            double roadLength = getRoadLength(roadName);
            if (roadLength <= 0 || position >= roadLength || position < 0 || cycle <= 0) {
                std::cerr << "Ongeldige positie/cycle voor verkeerslicht.\n";
                continue;
            }

            // === NIEUW: check geen tweede verkeerslicht binnen 50m ===
            bool tooClose = false;
            for (auto &existingLight : fTrafficLights) {
                if (existingLight->getRoadName() == roadName) {
                    double dist = std::fabs(existingLight->getPosition() - position);
                    if (dist < 50.0) {
                        tooClose = true;
                        break;
                    }
                }
            }
            if (tooClose) {
                std::cerr << "Inconsistent: verkeerslicht op baan " << roadName
                          << " (positie=" << position << "m)"
                          << " zit binnen 50m van een ander licht.\n";
                continue;
            }
            // ========================================================

            auto light = std::make_unique<TrafficLight>(roadName, position, cycle);
            fTrafficLights.push_back(std::move(light));
        }
        else if (elementType == "VOERTUIG") {
            std::string roadName;
            double position;
            ifs >> roadName >> position;
            double roadLen = getRoadLength(roadName);
            if (roadLen <= 0 || position >= roadLen || position < 0) {
                std::cerr << "Ongeldige positie voor voertuig.\n";
                continue;
            }
            auto vehicle = std::make_unique<Vehicle>(roadName, position);
            fVehicles.push_back(std::move(vehicle));
        }
        else if (elementType == "VOERTUIGGENERATOR") {
            std::string roadName;
            double frequency;
            ifs >> roadName >> frequency;
            bool alreadyExists = false;
            for (auto &gen : fGenerators) {
                if (gen->getRoadName() == roadName) {
                    alreadyExists = true;
                    std::cerr << "Er staat al een generator op deze baan!\n";
                    break;
                }
            }
            if (!alreadyExists && frequency > 0) {
                auto gen = std::make_unique<VehicleGenerator>(roadName, frequency);
                fGenerators.push_back(std::move(gen));
            }
        }
        else {
            std::cerr << "Onbekend elementtype: " << elementType << "\n";
            // overslaan
        }
    }
    ifs.close();
    return true;
}

void Simulation::runStep() {
    update();
}

void Simulation::loadScenario(const std::string &filename) {
    // leeg of eigen scenario-code
}

void Simulation::run() {
    while (fSimTime < kSimDuration) {
        update();
        outputState();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        fSimTime += kDeltaT;
    }
}

void Simulation::update() {
    // 1. Update verkeerslichten
    for (auto &light : fTrafficLights) {
        light->update(kDeltaT);
    }

    // 2. Check rood licht => stop of vertraag
    for (auto &vehicle : fVehicles) {
        TrafficLight* redLightAhead = nullptr;
        double minDist = 1e9;
        for (auto &light : fTrafficLights) {
            if (light->getRoadName() == vehicle->getRoadName() && !light->isGreen()) {
                double dist = light->getPosition() - vehicle->getPosition();
                if (dist > 0 && dist < minDist) {
                    minDist = dist;
                    redLightAhead = light.get();
                }
            }
        }
        if (redLightAhead) {
            if (minDist <= kStopDistance) {
                vehicle->stopImmediately();
            }
            else if (minDist <= kSlowDownDistance) {
                vehicle->applyTemporarySpeedFactor(kSlowFactor);
            } else {
                vehicle->resetMaxSpeed();
            }
        } else {
            vehicle->resetMaxSpeed();
        }
    }

    // 3. updateAcceleration + update
    for (auto &vehicle : fVehicles) {
        Vehicle* vehicleAhead = nullptr;
        for (auto &other : fVehicles) {
            if (other.get() != vehicle.get() &&
                other->getRoadName() == vehicle->getRoadName() &&
                other->getPosition() > vehicle->getPosition()) {
                if (!vehicleAhead || other->getPosition() < vehicleAhead->getPosition()) {
                    vehicleAhead = other.get();
                }
            }
        }
        vehicle->updateAcceleration(vehicleAhead);
        vehicle->update(kDeltaT);
    }

    // 4. Verwijder voertuigen buiten de weg
    fVehicles.erase(std::remove_if(fVehicles.begin(), fVehicles.end(),
        [this](const std::unique_ptr<Vehicle>& vehicle) {
            double pos = vehicle->getPosition();
            double roadLength = getRoadLength(vehicle->getRoadName());
            return (pos < 0.0 || pos >= roadLength);
        }), fVehicles.end());

    // 5. VehicleGenerators checken
    for (auto &gen : fGenerators) {
        gen->update(kDeltaT);
        if (gen->shouldGenerate()) {
            bool canSpawn = true;
            double spawnZone = 2.0 * 4.0; // 2*l
            for (auto &veh : fVehicles) {
                if (veh->getRoadName() == gen->getRoadName()) {
                    double dist = veh->getPosition();
                    if (dist >= 0.0 && dist <= spawnZone) {
                        canSpawn = false;
                        break;
                    }
                }
            }
            if (canSpawn) {
                fVehicles.push_back(std::make_unique<Vehicle>(gen->getRoadName(), 0.0));
            }
            gen->resetTimer();
        }
    }
}

void Simulation::outputState() const {
    std::cout << "Simulatietijd: " << fSimTime << "\n";
    for (const auto &vehicle : fVehicles) {
        std::cout << "Voertuig op " << vehicle->getRoadName()
                  << " positie: " << vehicle->getPosition()
                  << " snelheid: " << vehicle->getSpeed() << "\n";
    }
    for (const auto &light : fTrafficLights) {
        std::cout << "Verkeerslicht op " << light->getRoadName()
                  << " positie: " << light->getPosition()
                  << " is " << (light->isGreen() ? "Groen" : "Rood") << "\n";
    }
    std::cout << "-------------------------\n";
}

double Simulation::getRoadLength(const std::string &roadName) const {
    for (const auto &road : fRoads) {
        if (road->getName() == roadName) {
            return road->getLength();
        }
    }
    return 0.0;
}
