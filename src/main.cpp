#include "Simulation.h"

// Main function: initializes the simulation, loads input from file, and runs the simulation loop
int main() {
    // Simulation sim = parser.parse();
    Simulation sim;
    sim.loadFromFile("../tests/test_files/test_input.xml");
    sim.run();
    return 0;
}
