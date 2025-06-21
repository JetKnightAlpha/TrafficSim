#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include <vector>

class Road;
class Vehicle;
class TrafficLight;

/**
 * @brief The GraphicsEngine class handles rendering of the simulation's graphical state.
 *
 * Currently, rendering outputs text information to the console about roads, vehicles, and traffic lights.
 */
class GraphicsEngine {
public:
    /**
     * @brief Renders a simulation frame by displaying roads, vehicles, and traffic light statuses.
     *
     * @param roads Vector of pointers to roads in the simulation.
     * @param vehicles Vector of pointers to vehicles on the roads.
     * @param lights Vector of pointers to traffic lights controlling the traffic.
     */
    void render(const std::vector<Road*>& roads,
                const std::vector<Vehicle*>& vehicles,
                const std::vector<TrafficLight*>& lights) const;
};

#endif
