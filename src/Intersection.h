#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <string>
#include <vector>
#include <random>

class Vehicle;
class Road;

class Intersection {
public:
    struct EntryExit {
        std::string entryRoad;
        double entryPosition;
        std::string exitRoad;
        double exitPosition;

        EntryExit(const std::string& entry, double entryPos, const std::string& exit, double exitPos)
            : entryRoad(entry), entryPosition(entryPos), exitRoad(exit), exitPosition(exitPos) {}
    };

    Intersection(const std::vector<EntryExit>& entryExits);

    bool isOn(const Vehicle& v) const;
    void handleRoadSwitch(Vehicle& vehicle);
    void setRoads(const std::vector<Road*>& allRoads);

private:
    std::vector<EntryExit> entryExits;
    std::vector<Road*> roads;
};

#endif
