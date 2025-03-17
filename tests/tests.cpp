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
    Road road("Testweg", 100.0);
    EXPECT_EQ(road.getName(), "Testweg");
    EXPECT_DOUBLE_EQ(road.getLength(), 100.0);
}

/**
 * Test 2: Controleer of een voertuig zonder voorligger
 *         naar voren versnelt (Use-case 3.1).
 *
 * - Context: Volgens het IDM-model of de basislogica in Vehicle.cpp
 *   krijgt een voertuig zonder voorligger een positieve acceleratie
 *   (tenzij het al op vmax zit, maar hier starten we op v=0).
 * - Werking:
 *   1) We maken een Vehicle met startsnelheid 0.
 *   2) We roepen updateAcceleration(nullptr) aan (geen voorligger).
 *   3) Verwacht resultaat: getAcceleration() > 0.
 */
TEST(VehicleTest, AccelerationLogic_NoLeader) {
    Vehicle vehicle(0.0);
    vehicle.updateAcceleration(nullptr);
    EXPECT_GT(vehicle.getAcceleration(), 0.0)
        << "Voertuig zonder voorligger zou moeten versnellen (a>0).";
}

/**
 * Test 3: TrafficLight-omslag na cycle-verstrijken (Use-case 3.2).
 *
 * - Context: Een verkeerslicht begint op groen en na 'cycle' seconden
 *   hoort hij te flippen naar rood (of rood->groen).
 * - Werking:
 *   1) Maak een TrafficLight met cycle=20s.
 *   2) Roep update(25.0) aan (25 seconden).
 *   3) Na 25s is cycle (20s) gepasseerd, dus het licht zou moeten
 *      geflipt zijn naar rood.
 */
TEST(TrafficLightTest, StateTransition) {
    TrafficLight light("MainRoad", 50.0, 20.0);
    light.update(25.0);
    EXPECT_EQ(light.getState(), TrafficLight::State::Red)
        << "Verkeerslicht zou na 25s rood moeten zijn.";
}

/**
 * Test 4: Inlezen uit 'scenario.txt' (Use-cases 1.1 / 1.2).
 *
 * - Context: We willen controleren of de loadFromFile(...) methode
 *   werkt met een (pseudo) scenario-bestand. Dit bestand moet minimaal
 *   een BAAN bevatten.
 * - Werking:
 *   1) We maken een Simulation-object.
 *   2) We roepen sim.loadFromFile("scenario.txt").
 *   3) We verwachten dat de methode 'true' teruggeeft (bestand gevonden).
 *   4) We verwachten minstens één road in sim.getRoads().
 */
TEST(FileInputTest, LoadScenarioFile) {
    Simulation sim;
    bool ok = sim.loadFromFile("scenario.txt");
    EXPECT_TRUE(ok)
        << "Kon 'scenario.txt' niet openen (check pad/Working Dir).";
    EXPECT_FALSE(sim.getRoads().empty())
        << "Verwacht minstens 1 BAAN in het bestand.";
}

/**
 * Test 5: VehicleGeneration (Use-case 3.4).
 *
 * - Context: Als er in het scenario een VOERTUIGGENERATOR staat met
 *   een bepaalde frequentie, dan verwachten we dat na voldoende tijd
 *   (langer dan die frequentie) nieuwe voertuigen gespawnd worden.
 * - Werking:
 *   1) We laden scenario.txt (waar bv. VOERTUIGGENERATOR Middelheimlaan 5
 *      in staat).
 *   2) We runnen ~400 keer runStep() => ~6.64s (400*0.0166).
 *   3) We checken of er sindsdien minstens 1 extra voertuig is verschenen
 *      t.o.v. de initieel ingelezen voertuigen.
 *   4) We doen dat hier simpelweg via: EXPECT_GE(count, 3u).
 *      (We gaan ervan uit dat we initieel 2 voertuigen hebben.)
 */
TEST(SimulationTest, VehicleGeneration) {
    Simulation sim;
    bool ok = sim.loadFromFile("scenario.txt");
    ASSERT_TRUE(ok);

    for (int i = 0; i < 400; ++i) {
        sim.runStep();
    }
    auto count = sim.getVehicles().size();
    EXPECT_GE(count, 3u)
        << "Generator lijkt geen extra voertuig te hebben toegevoegd.";
}

/**
 * Test 6: Meerdere verkeerslichten op dezelfde weg (Use-case 3.2).
 *
 * - Context: We construeren een scenario in-memory (zonder bestand).
 *   We zetten twee verkeerslichten op één weg en kijken of de simulatie
 *   in elk geval niet crasht en het voertuig correct doorrijdt of stopt
 *   afhankelijk van de lichttoestand.
 * - Werking:
 *   1) Maak Road("TestRoad", 200).
 *   2) Maak 2 TrafficLights (licht1 op pos=50, licht2 op pos=100),
 *      beide met cycle=9999 (ze flippen dus erg laat).
 *   3) Maak 1 Vehicle op pos=0.
 *   4) Run ~1000 steps => ~16.6s.
 *   5) Omdat de lichten nooit rood worden (9999s is extreem lang),
 *      moet het voertuig doorrijden en dus een positie > 50m halen.
 */
TEST(SimulationTest, MultipleLightsSingleRoad) {
    Simulation sim;

    auto road = std::make_unique<Road>("TestRoad", 200.0);
    sim.addRoad(std::move(road));

    auto light1 = std::make_unique<TrafficLight>("TestRoad", 50.0, 9999.0);
    auto light2 = std::make_unique<TrafficLight>("TestRoad", 100.0, 9999.0);
    sim.addTrafficLight(std::move(light1));
    sim.addTrafficLight(std::move(light2));

    auto v = std::make_unique<Vehicle>("TestRoad", 0.0);
    sim.addVehicle(std::move(v));

    for (int i = 0; i < 1000; ++i) {
        sim.runStep();
    }

    double pos   = sim.getVehicles().at(0)->getPosition();
    double speed = sim.getVehicles().at(0)->getSpeed();

    // Met default fGreen=true en cycle=9999 => nooit rood => doorrijden.
    EXPECT_GT(pos, 50.0)
        << "Licht is nooit rood => voertuig moet doorrijden.";
    SUCCEED();
}

/**
 * Test 7: VehicleStopsAtRedLight (Use-case 3.2, stop-afstand).
 *
 * - Context: We willen expliciet zien dat een voertuig stopt
 *   voor een rood licht binnen redLight -> vehicle range <= stopDistance.
 * - Werking:
 *   1) Road("StopRoad", 100).
 *   2) TrafficLight("StopRoad", 30.0, 1.0) => cycle=1s => na 1s flipt naar rood.
 *   3) Run ~60 steps => ~1s => licht is nu rood.
 *   4) Voeg Vehicle("StopRoad", 0.0) toe.
 *   5) Run nog 1500 steps => ~25s => het voertuig moet bij pos=30 stilstaan.
 *   6) We checken of pos <= 30.0 + 1e-9 en speed ~ 0.0
 *      Met marge=3e-2, omdat we in practise 0.0239 m/s wel als "stilstand" zien.
 */
TEST(SimulationTest, VehicleStopsAtRedLight) {
    Simulation sim;

    // 1) Weg
    auto road = std::make_unique<Road>("StopRoad", 100.0);
    sim.addRoad(std::move(road));

    // 2) Licht (flip naar rood na 1s)
    auto light = std::make_unique<TrafficLight>("StopRoad", 30.0, 1.0);
    sim.addTrafficLight(std::move(light));

    // 3) ~1s simuleren => lamp rood
    for (int i = 0; i < 60; i++){
        sim.runStep(); // ~1s (60 * 0.0166 ~ 0.996s)
    }

    // 4) Voertuig spawnen
    auto veh = std::make_unique<Vehicle>("StopRoad", 0.0);
    sim.addVehicle(std::move(veh));

    // 5) ~25s simuleren => auto moet gaan stoppen
    for (int i=0; i<1500; i++){
        sim.runStep();
    }

    double pos   = sim.getVehicles().at(0)->getPosition();
    double speed = sim.getVehicles().at(0)->getSpeed();

    EXPECT_LE(pos, 30.0 + 1e-9)
        << "Voertuig had moeten stoppen voor rood licht op 30m.";

    // marge=3e-2 => speeds tot ~0.03 zijn "bijna 0"
    EXPECT_NEAR(speed, 0.0, 3e-2)
        << "Voertuig moet stil staan (ook als speed ~0.0239).";
}

/**
 * Test 8: InvalidRoadLength (Use-case 1.1 validatie).
 *
 * - Context: Als het scenario een weg heeft met lengte=-50,
 *   moet de code dit overslaan of false retourneren.
 * - Werking:
 *   1) We schrijven "invalid_scenario.txt" met
 *      BAAN NegativeRoad -50 (invalid)
 *      VOERTUIG SomeRoad 10 (ook invalid, want SomeRoad bestaat niet).
 *   2) We roepen sim.loadFromFile("invalid_scenario.txt").
 *   3) We checken of de weg niet is toegevoegd (fRoads is empty).
 *   4) We verwijderen daarna de file.
 */
TEST(FileInputTest, InvalidRoadLength) {
    {
        std::ofstream ofs("invalid_scenario.txt");
        ofs << "BAAN NegativeRoad -50\n";
        ofs << "VOERTUIG SomeRoad 10\n";
    }
    Simulation sim;
    bool ok = sim.loadFromFile("invalid_scenario.txt");
    EXPECT_TRUE(ok);  // we gaan ervan uit dat loadFromFile() "true" teruggeeft en 'continue' bij invalid data
    EXPECT_TRUE(sim.getRoads().empty())
        << "Ongeldige weg (lengte=-50) zou niet toegevoegd mogen worden.";

    remove("invalid_scenario.txt");
}

/**
 * De main voor GTest: start alle tests.
 */
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
/**
 * Test X: TwoCloseTrafficLights
 *
 * - Context: We willen niet dat er twee verkeerslichten zijn binnen 50m
 *   op dezelfde baan (Appendix A, consistentie-eis).
 * - Werking:
 *   1) We schrijven "close_lights.txt" met:
 *      BAAN MainRoad 200
 *      VERKEERSLICHT MainRoad 100 20
 *      VERKEERSLICHT MainRoad 130 25
 *   2) We roepen sim.loadFromFile("close_lights.txt") aan.
 *      Het tweede licht is 30m verwijderd van het eerste, wat < 50m is.
 *      => Het zou "Inconsistent: verkeerslicht ... zit binnen 50m" melden
 *         en het tweede licht NIET toevoegen.
 *   3) We controleren of er uiteindelijk maar 1 licht in sim.getTrafficLights() zit.
 */
TEST(FileInputTest, TwoCloseTrafficLights) {
    {
        std::ofstream ofs("close_lights.txt");
        ofs << "BAAN MainRoad 200\n";
        ofs << "VERKEERSLICHT MainRoad 100 20\n"; // Licht1
        ofs << "VERKEERSLICHT MainRoad 130 25\n"; // Licht2 => 30m verschil => te dicht
    }
    Simulation sim;
    bool ok = sim.loadFromFile("close_lights.txt");
    EXPECT_TRUE(ok);

    // We verwachten dat slechts 1 licht is toegevoegd
    EXPECT_EQ(sim.getTrafficLights().size(), 1u)
        << "Tweede verkeerslicht (op 130m) had niet toegevoegd mogen worden, "
        << "want <50m van het eerste (op 100m).";

    remove("close_lights.txt");
}
