#include "GraphicsEngine.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include <iostream>

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
