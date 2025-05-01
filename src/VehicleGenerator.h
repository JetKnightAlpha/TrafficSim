#ifndef VEHICLEGENERATOR_H
#define VEHICLEGENERATOR_H

class Road;

class VehicleGenerator {
public:
    VehicleGenerator(Road* road, int frequency);

    void update(double currentTime);
private:
    Road* road;
    int frequency;
    double lastGenerated;
};

#endif
