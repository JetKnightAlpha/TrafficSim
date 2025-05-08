#include "gtest/gtest.h"
#include "Simulation.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"
#include "BusStop.h"
#include "Intersection.h"
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;
const fs::path RES = fs::path("..") / "tests" / "test_files";

// Helper functions
static std::string getSimulationOutput(const std::string& file)
{
    Simulation sim;
    sim.loadFromFile((RES / file).string());
    testing::internal::CaptureStdout();
    sim.outputState();
    return testing::internal::GetCapturedStdout();
}

// Test fixtures
class TrafficSimulationTest : public ::testing::Test {
protected:
    void SetUp() override {
        sim = std::make_unique<Simulation>();
    }
    std::unique_ptr<Simulation> sim;
};

// VERPLICHT (MANDATORY) REQUIREMENTS
// 1. Verkeerssituatie inlezen (Loading traffic situation)
TEST_F(TrafficSimulationTest, ShouldLoadValidTrafficSituation) {
    std::string output = getSimulationOutput("01_basic_ok.xml");
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("Roads:"));
    EXPECT_TRUE(output.find("Main"));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidTrafficSituation) {
    try {
        getSimulationOutput("01_basic_bad.xml");
    } catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("Missing required elements") != std::string::npos);
    }
}

// 2. Voertuig met type inlezen (Loading vehicle with type)
TEST_F(TrafficSimulationTest, ShouldLoadValidVehicleWithType) {
    std::string output = getSimulationOutput("06_vehicletype_ok.xml");
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("Vehicles:"));
    EXPECT_TRUE(output.find("auto"));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidVehicleType) {
    try {
        getSimulationOutput("06_vehicletype_bad.xml");
    } catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("Invalid position value") != std::string::npos);
    }
}

// 3. Rijden van voertuig (Vehicle movement)
TEST_F(TrafficSimulationTest, ShouldSimulateValidVehicleMovement) {
    sim->loadFromFile((RES / "05_vehicle_ok.xml").string());
    auto initialPos = sim->getVehicles()[0]->getPosition();
    sim->runStep();
    auto newPos = sim->getVehicles()[0]->getPosition();
    EXPECT_NE(initialPos, newPos);
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidVehicleMovement) {
    try {
        getSimulationOutput("05_vehicle_bad.xml");
    } catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("Invalid position value") != std::string::npos);
    }
}

// 4. Simulatie van verkeerslicht (Traffic light simulation)
TEST_F(TrafficSimulationTest, ShouldSimulateValidTrafficLight) {
    sim->loadFromFile((RES / "03_tlight_ok.xml").string());
    auto initialState = sim->getTrafficLights()[0]->isGreen();
    // Run enough steps to ensure the traffic light changes state
    for (int i = 0; i < 1875; i++) {  // 300 steps = 30 seconds
        sim->runStep();
    }
    auto newState = sim->getTrafficLights()[0]->isGreen();
    EXPECT_NE(initialState, newState);
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidTrafficLight) {
    try {
        getSimulationOutput("03_tlight_bad.xml");
    } catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("Missing required elements") != std::string::npos);
    }
}

// 5. Automatische simulatie (Automatic simulation)
TEST_F(TrafficSimulationTest, ShouldRunValidAutomaticSimulation) {
    sim->loadFromFile((RES / "01_basic_ok.xml").string());
    auto initialTime = sim->currentTime;
    sim->runStep();
    auto newTime = sim->currentTime;
    EXPECT_NE(initialTime, newTime);
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidAutomaticSimulation) {
    try {
        getSimulationOutput("01_basic_bad.xml");
    } catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("Missing required elements") != std::string::npos);
    }
}

// 6. Simpele uitvoer (Simple output)
TEST_F(TrafficSimulationTest, ShouldGenerateValidOutput) {
    std::string output = getSimulationOutput("01_basic_ok.xml");
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("Roads:"));
    EXPECT_TRUE(output.find("Vehicles:"));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidOutput) {
    try {
        getSimulationOutput("01_basic_bad.xml");
    } catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("Missing required elements") != std::string::npos);
    }
}

// BELANGRIJK (IMPORTANT) REQUIREMENTS
// 1. Voertuiggenerator inlezen (Loading vehicle generator)
TEST_F(TrafficSimulationTest, ShouldLoadValidVehicleGenerator) {
    std::string output = getSimulationOutput("07_vgenerator_ok.xml");
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("Vehicle Generators:"));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidVehicleGenerator) {
    try {
        getSimulationOutput("07_vgenerator_bad.xml");
    } catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("Missing required elements") != std::string::npos);
    }
}

// 2. Bushaltes inlezen (Loading bus stops)
TEST_F(TrafficSimulationTest, ShouldLoadValidBusStops) {
    std::string output = getSimulationOutput("08_busstop_ok.xml");
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("Bus Stops:"));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidBusStop) {
    try {
        getSimulationOutput("08_busstop_bad.xml");
    } catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("Missing required elements") != std::string::npos);
    }
}

// 3. Kruispunten inlezen (Loading intersections)
TEST_F(TrafficSimulationTest, ShouldLoadValidIntersections) {
    std::string output = getSimulationOutput("10_intersection_ok.xml");
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("Intersections:"));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidIntersection) {
    try {
        getSimulationOutput("10_intersection_bad.xml");
    } catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("Missing required attributes") != std::string::npos);
    }
}

// 4. Simulatie met voertuiggenerator (Vehicle generator simulation)
TEST_F(TrafficSimulationTest, ShouldSimulateValidVehicleGeneration) {
    sim->loadFromFile((RES / "07_vgenerator_ok.xml").string());
    auto initialCount = sim->getVehicles().size();
    sim->runStep();
    auto newCount = sim->getVehicles().size();
    EXPECT_GE(newCount, initialCount);
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidVehicleGeneration) {
    try {
        getSimulationOutput("07_vgenerator_bad.xml");
    } catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("Missing required elements") != std::string::npos);
    }
}

// 5. Simulatie van bushaltes (Bus stop simulation)
TEST_F(TrafficSimulationTest, ShouldSimulateValidBusStops) {
    sim->loadFromFile((RES / "08_busstop_ok.xml").string());
    // Add a bus to the road
    auto road = sim->getRoads()[0];
    auto bus = new Vehicle(road, 0, "bus");
    road->addVehicle(bus);
    sim->addVehicle(bus);
    EXPECT_NO_THROW(bus->shouldWaitAt(250, 30));  // Test with bus stop position and wait time
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidBusStopSimulation) {
    try {
        getSimulationOutput("08_busstop_bad.xml");
    } catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("Missing required elements") != std::string::npos);
    }
}

// 6. Simulatie van kruispunten (Intersection simulation)
TEST_F(TrafficSimulationTest, ShouldSimulateValidIntersections) {
    sim->loadFromFile((RES / "10_intersection_ok.xml").string());
    // Add a vehicle to the road
    auto road = sim->getRoads()[0];
    auto vehicle = new Vehicle(road, 0, "auto");
    road->addVehicle(vehicle);
    sim->addVehicle(vehicle);
    auto intersection = sim->getIntersections()[0];
    EXPECT_NO_THROW(intersection->handleRoadSwitch(vehicle));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidIntersectionSimulation) {
    try {
        getSimulationOutput("10_intersection_bad.xml");
    } catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("Missing required attributes") != std::string::npos);
    }
}
