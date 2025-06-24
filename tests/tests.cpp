#include "gtest/gtest.h"
#include "Simulation.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"
#include "BusStop.h"
#include "Intersection.h"
#include "Parser.h"
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;
const fs::path RES = fs::path("..") / "tests" / "test_files";

static std::unique_ptr<Simulation> loadFromFile(const std::string& file)
{
    auto sim = std::make_unique<Simulation>();

    std::vector<Road*> roads;
    std::vector<VehicleGenerator*> generators;
    std::vector<BusStop*> busStops;
    std::vector<Intersection*> intersections;

    Parser::parseFile((RES / file).string(), roads, generators, busStops, intersections);

    for (auto* road : roads)
        sim->addRoad(road);
    for (auto* gen : generators)
        sim->addGenerator(gen);
    for (auto* bs : busStops)
        sim->addBusStop(bs);
    for (auto* isec : intersections)
        sim->addIntersection(isec);
    for (auto* road : roads) {
        for (auto* vehicle : road->getVehicles())
            sim->addVehicle(vehicle);
        for (auto* light : road->getTrafficLights())
            sim->addTrafficLight(light);
    }

    return sim;
}


// Helper functions
static std::string getSimulationOutput(const std::string& file)
{
    auto sim = loadFromFile(file);
    testing::internal::CaptureStdout();
    sim->outputState();
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

// 1. Verkeerssituatie inlezen
TEST_F(TrafficSimulationTest, ShouldLoadValidTrafficSituation) {
    std::string output = getSimulationOutput("01_basic_ok.xml");
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("Roads:"));
    EXPECT_TRUE(output.find("Main"));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidTrafficSituation) {
    try {
        getSimulationOutput("01_basic_bad.xml");
        FAIL() << "Expected invalid XML to be rejected";
    } catch (const std::runtime_error& e) {
        SUCCEED();
    }
}

// 2. Voertuig met type inlezen
TEST_F(TrafficSimulationTest, ShouldLoadValidVehicleWithType) {
    std::string output = getSimulationOutput("06_vehicletype_ok.xml");
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("Vehicles:"));
    EXPECT_TRUE(output.find("auto"));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidVehicleType) {
    try {
        getSimulationOutput("06_vehicletype_bad.xml");
        FAIL() << "Expected invalid vehicle XML to be rejected";
    } catch (const std::runtime_error& e) {
        SUCCEED();
    }
}

// 3. Rijden van voertuig
TEST_F(TrafficSimulationTest, ShouldSimulateValidVehicleMovement) {
    sim = loadFromFile("05_vehicle_bad.xml");
    auto initialPos = sim->getVehicles()[0]->getPosition();
    sim->runStep();
    auto newPos = sim->getVehicles()[0]->getPosition();
    EXPECT_NE(initialPos, newPos);
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidVehicleMovement) {
    try {
        getSimulationOutput("05_vehicle_bad.xml");
        FAIL() << "Expected invalid vehicle movement XML to be rejected";
    } catch (const std::runtime_error&) {
        SUCCEED();
    }
}

// 4. Simulatie van verkeerslicht
TEST_F(TrafficSimulationTest, ShouldSimulateValidTrafficLight) {
    sim = loadFromFile("03_tlight_ok.xml");
    auto initialState = sim->getTrafficLights()[0]->isGreen();
    for (int i = 0; i < 1875; i++) {
        sim->runStep();
    }
    auto newState = sim->getTrafficLights()[0]->isGreen();
    EXPECT_NE(initialState, newState);
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidTrafficLight) {
    try {
        getSimulationOutput("03_tlight_bad.xml");
        FAIL() << "Expected invalid traffic light XML to be rejected";
    } catch (const std::runtime_error&) {
        SUCCEED();
    }
}

// 5. Automatische simulatie
TEST_F(TrafficSimulationTest, ShouldRunValidAutomaticSimulation) {
    sim = loadFromFile("01_basic_ok.xml");
    auto initialTime = sim->currentTime;
    sim->runStep();
    auto newTime = sim->currentTime;
    EXPECT_NE(initialTime, newTime);
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidAutomaticSimulation) {
    try {
        getSimulationOutput("01_basic_bad.xml");
        FAIL() << "Expected invalid automatic simulation XML to be rejected";
    } catch (const std::runtime_error&) {
        SUCCEED();
    }
}

// 6. Simpele uitvoer
TEST_F(TrafficSimulationTest, ShouldGenerateValidOutput) {
    std::string output = getSimulationOutput("01_basic_ok.xml");
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("Roads:"));
    EXPECT_TRUE(output.find("Vehicles:"));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidOutput) {
    try {
        getSimulationOutput("01_basic_bad.xml");
        FAIL() << "Expected invalid output XML to be rejected";
    } catch (const std::runtime_error&) {
        SUCCEED();
    }
}

// BELANGRIJK (IMPORTANT) REQUIREMENTS

// 1. Voertuiggenerator inlezen
TEST_F(TrafficSimulationTest, ShouldLoadValidVehicleGenerator) {
    std::string output = getSimulationOutput("07_vgenerator_ok.xml");
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("Vehicle Generators:"));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidVehicleGenerator) {
    try {
        getSimulationOutput("07_vgenerator_bad.xml");
        FAIL() << "Expected invalid vehicle generator XML to be rejected";
    } catch (const std::runtime_error&) {
        SUCCEED();
    }
}

// 2. Bushaltes inlezen
TEST_F(TrafficSimulationTest, ShouldLoadValidBusStops) {
    std::string output = getSimulationOutput("08_busstop_ok.xml");
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("Bus Stops:"));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidBusStop) {
    try {
        getSimulationOutput("08_busstop_bad.xml");
        FAIL() << "Expected invalid bus stop XML to be rejected";
    } catch (const std::runtime_error&) {
        SUCCEED();
    }
}

// 3. Kruispunten inlezen
TEST_F(TrafficSimulationTest, ShouldLoadValidIntersections) {
    std::string output = getSimulationOutput("10_intersection_ok.xml");
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("Intersections:"));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidIntersection) {
    try {
        getSimulationOutput("10_intersection_bad.xml");
        FAIL() << "Expected invalid intersection XML to be rejected";
    } catch (const std::runtime_error&) {
        SUCCEED();
    }
}

// 4. Simulatie met voertuiggenerator
TEST_F(TrafficSimulationTest, ShouldSimulateValidVehicleGeneration) {
    sim = loadFromFile("07_vgenerator_ok.xml");
    auto initialCount = sim->getVehicles().size();
    sim->runStep();
    auto newCount = sim->getVehicles().size();
    EXPECT_GE(newCount, initialCount);
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidVehicleGeneration) {
    try {
        getSimulationOutput("07_vgenerator_bad.xml");
        FAIL() << "Expected invalid vehicle generation XML to be rejected";
    } catch (const std::runtime_error&) {
        SUCCEED();
    }
}

// 5. Simulatie van bushaltes
TEST_F(TrafficSimulationTest, ShouldSimulateValidBusStops) {
    sim = loadFromFile("08_busstop_ok.xml");
    auto road = sim->getRoads()[0];
    auto bus = new Bus (road, 0);
    road->addVehicle(bus);
    sim->addVehicle(bus);
    EXPECT_NO_THROW(bus->shouldWaitAt(250, 30));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidBusStopSimulation) {
    try {
        getSimulationOutput("08_busstop_bad.xml");
        FAIL() << "Expected invalid bus stop simulation XML to be rejected";
    } catch (const std::runtime_error&) {
        SUCCEED();
    }
}

// 6. Simulatie van kruispunten
TEST_F(TrafficSimulationTest, ShouldSimulateValidIntersections) {
    sim = loadFromFile("10_intersection_ok.xml");
    auto road = sim->getRoads()[0];
    auto vehicle = new Auto(road, 0);
    road->addVehicle(vehicle);
    sim->addVehicle(vehicle);
    auto intersection = sim->getIntersections()[0];
    EXPECT_NO_THROW(intersection->handleRoadSwitch(vehicle));
}

TEST_F(TrafficSimulationTest, ShouldFailOnInvalidIntersectionSimulation) {
    try {
        getSimulationOutput("10_intersection_bad.xml");
        FAIL() << "Expected invalid intersection simulation XML to be rejected";
    } catch (const std::runtime_error&) {
        SUCCEED();
    }
}
