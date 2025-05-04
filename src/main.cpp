#include "Simulation.h"

int main() {
    Simulation sim;
    sim.loadFromFile("../tests/test_files/test_input.xml");
    sim.run();
    return 0;
}
