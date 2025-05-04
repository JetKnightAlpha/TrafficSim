#include "Intersection.h"
#include "Vehicle.h"
#include "Road.h"
#include <random>
#include <cmath>

Intersection::Intersection(const std::vector<Entry>& entries)
    : fEntries(entries) {}

bool Intersection::isOn(const Vehicle& v) const {
    for (const auto& entry : fEntries) {
        if (entry.first == v.getRoad()->getName() &&
            std::fabs(entry.second - v.getPosition()) < 0.05) {
            return true;
            }
    }
    return false;
}

std::optional<Intersection::Entry> Intersection::getNextRoad(const std::string& currentRoad) const {
    if (fEntries.size() < 2) return std::nullopt;

    std::vector<Entry> options;
    for (const auto& entry : fEntries) {
        if (entry.first != currentRoad) {
            options.push_back(entry);
        }
    }

    if (options.empty()) return std::nullopt;

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, options.size() - 1);
    return options[dist(rng)];
}
