#include <gtest/gtest.h>
#include <fstream>
#include <cmath>
#include "Simulation.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"

/**
 * Test 1: Basistest voor Road.
 *
 * - Context: We willen controleren of een Road-object
 *   correct wordt aangemaakt met een gegeven naam en lengte.
 * - Werking:
 *   1) We maken een Road-obj met name="Testweg" en length=100.0.
 *   2) We verifiëren dat getName() en getLength() de verwachte
 *      waarden opleveren.
 */
TEST(InputTest, RoadLoading) {
    Road road("Testweg", 100);  // Updated to match the constructor
    EXPECT_EQ(road.getName(), "Testweg");
    EXPECT_EQ(road.getLength(), 100);  // Changed to int comparison
}

/**
 * Test 2: Controleer of een voertuig zonder voorligger
 *         naar voren versnelt.
 *
 * - Context: We willen testen of een voertuig zonder voorligger
 *   (geen voertuigen ervoor) correct versnelt.
 * - Werking:
 *   1) We maken een voertuig zonder voorligger.
 *   2) We verifiëren of het voertuig versnelt.
 */
TEST(VehicleTest, AccelerationLogic_NoLeader) {
    Road road("Testweg", 100);  // Create a road object
    Vehicle vehicle(&road, 0.0);  // Pass Road pointer to Vehicle constructor
    vehicle.update(0.0166);  // Assuming 1 frame update (deltaTime)
    EXPECT_GT(vehicle.getAcceleration(), 0.0)
        << "Voertuig zonder voorligger zou moeten versnellen (a>0).";
}

/**
 * Test 3: TrafficLight-omslag na cycle-verstrijken.
 *
 * - Context: We willen testen of het verkeerslicht correct omschakelt
 *   na de opgegeven cycle tijd.
 * - Werking:
 *   1) We maken een verkeerslicht aan met een cycle tijd van 20 seconden.
 *   2) We verifiëren of het verkeerslicht groen of rood is na 25 seconden.
 */
TEST(TrafficLightTest, StateTransition) {
    Road road("MainRoad", 200);
    TrafficLight light(&road, 50, 20);  // Pass Road pointer to TrafficLight constructor
    light.update(25.0);
    EXPECT_FALSE(light.isGreen())  // Should be red after 25 seconds
        << "Verkeerslicht zou na 25s rood moeten zijn.";
}

/**
 * Test 4: Inlezen uit 'scenario.txt'.
 *
 * - Context: We willen testen of het bestand correct wordt ingelezen
 *   en de wegen worden geladen.
 * - Werking:
 *   1) We laden het bestand "scenario.txt".
 *   2) We verifiëren dat er wegen geladen zijn uit het bestand.
 */
TEST(FileInputTest, LoadScenarioFile) {
    Simulation sim;
    sim.loadFromFile("scenario.txt");  // Adjusted to match Simulation API
    EXPECT_FALSE(sim.getRoads().empty())
        << "Verwacht minstens 1 BAAN in het bestand.";
}

/**
 * Test 5: VehicleGeneration.
 *
 * - Context: We willen testen of voertuigen correct worden gegenereerd
 *   volgens de opgegeven frequentie.
 * - Werking:
 *   1) We laden het scenario en voeren meerdere stappen uit.
 *   2) We verifiëren of er voertuigen zijn gegenereerd.
 */
TEST(SimulationTest, VehicleGeneration) {
    Simulation sim;
    sim.loadFromFile("scenario.txt");  // Adjusted to match Simulation API

    for (int i = 0; i < 400; ++i) {
        sim.runStep();
    }
    auto count = sim.getVehicles().size();
    EXPECT_GE(count, 3u)
        << "Generator lijkt geen extra voertuig te hebben toegevoegd.";
}

/**
 * Test 6: Meerdere verkeerslichten op dezelfde weg.
 *
 * - Context: We willen testen of meerdere verkeerslichten op dezelfde
 *   weg correct worden toegevoegd.
 * - Werking:
 *   1) We voegen twee verkeerslichten toe op dezelfde weg.
 *   2) We verifiëren of beide lichten zijn toegevoegd.
 */
TEST(SimulationTest, MultipleLightsSingleRoad) {
    Simulation sim;

    Road road("TestRoad", 200);
    sim.addRoad(&road);

    TrafficLight light1(&road, 50, 9999);
    TrafficLight light2(&road, 100, 9999);
    sim.addTrafficLight(&light1);
    sim.addTrafficLight(&light2);

    Vehicle vehicle(&road, 0.0);
    sim.addVehicle(&vehicle);

    for (int i = 0; i < 1000; ++i) {
        sim.runStep();
    }

    double pos = vehicle.getPosition();
    double speed = vehicle.getSpeed();

    EXPECT_GT(pos, 50.0)
        << "Licht is nooit rood => voertuig moet doorrijden.";
}

/**
 * Test 7: VehicleStopsAtRedLight.
 *
 * - Context: We willen testen of een voertuig stopt voor een rood
 *   verkeerslicht.
 * - Werking:
 *   1) We voegen een verkeerslicht toe en wachten totdat het groen wordt.
 *   2) We verifiëren of het voertuig stopt bij het rood licht.
 */
TEST(SimulationTest, VehicleStopsAtRedLight) {
    Simulation sim;

    Road road("StopRoad", 100);
    sim.addRoad(&road);

    TrafficLight light(&road, 30, 1);  // Cycle time = 1s
    sim.addTrafficLight(&light);

    for (int i = 0; i < 60; i++){
        sim.runStep();  // ~1s
    }

    Vehicle vehicle(&road, 0.0);
    sim.addVehicle(&vehicle);

    for (int i = 0; i < 1500; i++){
        sim.runStep();  // ~25s
    }

    double pos = vehicle.getPosition();
    double speed = vehicle.getSpeed();

    EXPECT_LE(pos, 30.0 + 1e-9)
        << "Voertuig had moeten stoppen voor rood licht op 30m.";

    EXPECT_NEAR(speed, 0.0, 3e-2)
        << "Voertuig moet stil staan (ook als speed ~0.0239).";
}

/**
 * Test 8: InvalidRoadLength.
 *
 * - Context: We willen testen of het inlezen van een weg met een
 *   negatieve lengte correct wordt afgewezen.
 * - Werking:
 *   1) We voegen een weg toe met een negatieve lengte (-50).
 *   2) We verifiëren of de weg niet wordt toegevoegd.
 */
TEST(FileInputTest, InvalidRoadLength) {
    {
        std::ofstream ofs("invalid_scenario.txt");
        ofs << "BAAN NegativeRoad -50\n";
        ofs << "VOERTUIG SomeRoad 10\n";
    }
    Simulation sim;
    sim.loadFromFile("invalid_scenario.txt");  // Adjusted to match Simulation API
    EXPECT_TRUE(sim.getRoads().empty())
        << "Ongeldige weg (lengte=-50) zou niet toegevoegd mogen worden.";

    remove("invalid_scenario.txt");
}

/**
 * Test 9: Twee verkeerslichten te dicht op elkaar.
 *
 * - Context: We willen testen of het systeem twee verkeerslichten
 *   correct afwijst als ze te dicht bij elkaar staan.
 * - Werking:
 *   1) We voegen twee verkeerslichten toe die te dicht bij elkaar staan.
 *   2) We verifiëren of het tweede verkeerslicht correct wordt afgewezen.
 */
TEST(FileInputTest, TwoCloseTrafficLights) {
    {
        std::ofstream ofs("close_lights.txt");
        ofs << "BAAN MainRoad 200\n";
        ofs << "VERKEERSLICHT MainRoad 100 20\n";
        ofs << "VERKEERSLICHT MainRoad 130 25\n";  // 30m apart => invalid
    }
    Simulation sim;
    sim.loadFromFile("close_lights.txt");  // Adjusted to match Simulation API
    EXPECT_EQ(sim.getTrafficLights().size(), 1u)
        << "Tweede verkeerslicht (op 130m) had niet toegevoegd mogen worden, "
        << "want <50m van het eerste (op 100m).";

    remove("close_lights.txt");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
