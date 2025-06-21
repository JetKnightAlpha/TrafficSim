#include "GraphicsEngine.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include <iostream>

/**
 * @brief Renders the current simulation frame by printing details about roads, vehicles, and traffic lights.
 *
 * This function outputs to standard console:
 * - Number of roads present.
 * - Each vehicle's type, position (in meters), and speed (m/s).
 * - Each traffic light's position and whether it is green or red.
 *
 * @param roads Vector of pointers to Road objects in the simulation.
 * @param vehicles Vector of pointers to Vehicle objects currently on roads.
 * @param lights Vector of pointers to TrafficLight objects controlling traffic flow.
 */
void GraphicsEngine::render(const std::vector<Road*>& roads,
                            const std::vector<Vehicle*>& vehicles,
                            const std::vector<TrafficLight*>& lights) const {
    std::cout << "[GraphicsEngine] frame - " << roads.size() << " roads\n";

    for (const auto& vehicle : vehicles) {
        std::cout << "Vehicle [" << vehicle->getType() << "] at "
                  << vehicle->getPosition() << " m, speed: "
                  << vehicle->getSpeed() << " m/s\n";
    }

    for (const auto& light : lights) {
        std::cout << "TrafficLight at " << light->getPosition()
                  << " is " << (light->isGreen() ? "Green" : "Red") << "\n";
    }
}
