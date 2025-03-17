#ifndef INC_VEHICLEGENERATOR_H
#define INC_VEHICLEGENERATOR_H

#include <string>

class VehicleGenerator {
public:
    VehicleGenerator(const std::string &roadName, double frequency);
    void update(double deltaT);
    bool shouldGenerate() const;
    void resetTimer();
    std::string getRoadName() const;

private:
    std::string fRoadName;
    double fFrequency;
    double fTimeSinceLast;
};

#endif // INC_VEHICLEGENERATOR_H
