#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include <vector>
#include <memory>

class Road;
class Vehicle;
class TrafficLight;

class GraphicsEngine {
public:
    void render(const std::vector<Road*>& roads,
                const std::vector<Vehicle*>& vehicles,
                const std::vector<TrafficLight*>& lights) const;
};

#endif
