#include "Simulation.h"
#include <iostream>

int main() {
    Simulation sim;
    // Voorbeeld: scenario.txt inlezen
    bool ok = sim.loadFromFile("scenario.txt");
    if (!ok) {
        std::cerr << "Kon scenario.txt niet openen.\n";
    }

    // Start de simulatie
    sim.run();

    return 0;
}
