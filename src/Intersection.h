#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <string>
#include <vector>
#include <optional>
#include <utility>

class Vehicle;

class Intersection {
public:
    using Entry = std::pair<std::string, double>;

    explicit Intersection(const std::vector<Entry>& entries);

    bool isOn(const Vehicle& v) const;
    std::optional<Entry> getNextRoad(const std::string& currentRoad) const;

private:
    std::vector<Entry> fEntries;
};

#endif
