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
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>

namespace fs = std::filesystem;
const fs::path RES = fs::path("..") / "tests" / "test_files";
const fs::path EXPECTED_ERRORS = fs::path("..") / "tests" / "Expected_Testfiles";
const fs::path XML_TESTFILES = fs::path("..") / "tests" / "XML_Testfiles";

// Declare the global error handler
extern std::function<void(const std::string&, const char*, int)> g_errorHandler;

// Define the global error handler for tests
std::function<void(const std::string&, const char*, int)> g_errorHandler = [](const std::string& message, const char* file, int line) {
    std::cerr << "REQUIRE Violation: " << message << " in " << file << ":" << line << std::endl;
    std::exit(1);
};

static std::unique_ptr<Simulation> loadFromFile(const std::string& file)
{
    auto sim = std::make_unique<Simulation>();

    std::vector<Road*> roads;
    std::vector<VehicleGenerator*> generators;
    std::vector<BusStop*> busStops;
    std::vector<Intersection*> intersections;

    // Use the appropriate path based on file type
    std::string filePath;
    if (file.find("_bad.xml") != std::string::npos) {
        filePath = (XML_TESTFILES / file).string();
    } else {
        filePath = (RES / file).string();
    }

    Parser::parseFile(filePath, roads, generators, busStops, intersections);

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

// Helper function to normalize output (remove extra whitespace, normalize line endings)
static std::string normalizeOutput(const std::string& output) {
    std::string normalized = output;
    
    // Replace Windows line endings with Unix line endings
    size_t pos = 0;
    while ((pos = normalized.find("\r\n", pos)) != std::string::npos) {
        normalized.replace(pos, 2, "\n");
    }
    
    // Remove trailing whitespace from each line
    std::istringstream stream(normalized);
    std::ostringstream result;
    std::string line;
    
    while (std::getline(stream, line)) {
        // Remove trailing whitespace
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        result << line << "\n";
    }
    
    return result.str();
}

// Helper function to capture all output (stdout and stderr) from simulation
static std::string captureAllOutput(const std::string& xmlFile) {
    std::stringstream outputStream;
    
    // Redirect both stdout and stderr
    std::streambuf* originalStdout = std::cout.rdbuf();
    std::streambuf* originalStderr = std::cerr.rdbuf();
    std::cout.rdbuf(outputStream.rdbuf());
    std::cerr.rdbuf(outputStream.rdbuf());
    
    try {
        auto sim = loadFromFile(xmlFile);
        sim->outputState();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown error occurred" << std::endl;
    }
    
    // Restore original streams
    std::cout.rdbuf(originalStdout);
    std::cerr.rdbuf(originalStderr);
    
    return outputStream.str();
}

// Helper function to capture output including REQUIRE violations
static std::string captureOutputWithRequireViolations(const std::string& xmlFile) {
    std::stringstream outputStream;
    std::string capturedError;
    
    // Redirect both stdout and stderr
    std::streambuf* originalStdout = std::cout.rdbuf();
    std::streambuf* originalStderr = std::cerr.rdbuf();
    std::cout.rdbuf(outputStream.rdbuf());
    std::cerr.rdbuf(outputStream.rdbuf());
    
    // Set up custom error handler that captures the error message
    auto originalErrorHandler = g_errorHandler;
    g_errorHandler = [&capturedError](const std::string& message, const char* file, int line) {
        std::ostringstream errorMsg;
        errorMsg << "REQUIRE Violation: " << message << " in " << file << ":" << line;
        capturedError = errorMsg.str();
        // Don't exit, just capture the error
    };
    
    try {
        // Extract just the filename from the full path for loadFromFile
        std::string filename = xmlFile;
        size_t lastSlash = filename.find_last_of('/');
        if (lastSlash != std::string::npos) {
            filename = filename.substr(lastSlash + 1);
        }
        
        auto sim = loadFromFile(filename);
        sim->run(); // Run the full simulation instead of just outputState
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown error occurred" << std::endl;
    }
    
    // Restore original error handler
    g_errorHandler = originalErrorHandler;
    
    // Restore original streams
    std::cout.rdbuf(originalStdout);
    std::cerr.rdbuf(originalStderr);
    
    // Return captured error if any, otherwise return normal output
    if (!capturedError.empty()) {
        return capturedError + "\n";
    }
    return outputStream.str();
}

// Main comparison function
static void compare2(const std::string& expectedFile, const std::string& xmlFile) {
    // Convert relative paths to absolute WSL paths
    std::string expectedPath = expectedFile;
    std::string xmlPath = xmlFile;
    
    if (expectedFile.find("Expected_Testfiles/") == 0) {
        expectedPath = "../tests/" + expectedFile;
    }
    if (xmlFile.find("XML_Testfiles/") == 0) {
        xmlPath = "../tests/" + xmlFile;
    }
    
    // Load expected output
    std::ifstream expectedStream(expectedPath);
    if (!expectedStream.is_open()) {
        FAIL() << "Could not open expected output file: " << expectedPath;
    }
    std::stringstream expectedBuffer;
    expectedBuffer << expectedStream.rdbuf();
    std::string expectedOutput = expectedBuffer.str();
    
    // Capture actual output including REQUIRE violations
    std::string actualOutput = captureOutputWithRequireViolations(xmlPath);
    
    // Normalize both outputs
    std::string normalizedActualOutput = normalizeOutput(actualOutput);
    std::string normalizedExpectedOutput = normalizeOutput(expectedOutput);
    
    // Compare the normalized outputs
    EXPECT_EQ(normalizedActualOutput, normalizedExpectedOutput)
        << "Combined Output:\n"
        << "Original actual output:\n" << actualOutput
        << "\nOriginal expected output:\n" << expectedOutput;
}

// Helper function to compare error streams
static bool compareErrorStreams(const std::string& actual, const std::string& expected) {
    std::istringstream actualStream(actual);
    std::istringstream expectedStream(expected);
    
    std::string actualLine, expectedLine;
    int lineNumber = 1;
    
    while (std::getline(expectedStream, expectedLine)) {
        if (!std::getline(actualStream, actualLine)) {
            std::cout << "Expected more lines in actual output. Missing at line " << lineNumber << std::endl;
            return false;
        }
        
        // Remove trailing whitespace for comparison
        actualLine.erase(actualLine.find_last_not_of(" \t\r\n") + 1);
        expectedLine.erase(expectedLine.find_last_not_of(" \t\r\n") + 1);
        
        if (actualLine != expectedLine) {
            std::cout << "Mismatch at line " << lineNumber << ":" << std::endl;
            std::cout << "Expected: '" << expectedLine << "'" << std::endl;
            std::cout << "Actual:   '" << actualLine << "'" << std::endl;
            return false;
        }
        lineNumber++;
    }
    
    // Check if there are more lines in actual output
    if (std::getline(actualStream, actualLine)) {
        std::cout << "Actual output has more lines than expected starting at line " << lineNumber << std::endl;
        return false;
    }
    
    return true;
}

// Helper function to load expected errors from file
static std::string loadExpectedErrors(const std::string& testName) {
    std::string expectedErrorsFile = (EXPECTED_ERRORS / (testName + "_expected_errors.txt")).string();
    std::ifstream file(expectedErrorsFile);
    if (!file.is_open()) {
        return ""; // No expected errors file found
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Helper functions
static std::string getSimulationOutput(const std::string& file)
{
    auto sim = loadFromFile(file);
    testing::internal::CaptureStdout();
    sim->run(); // Run the full simulation instead of just outputState
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

// New test class for error comparison tests
class TrafficSimulationErrorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure the expected errors directory exists
        if (!fs::exists(EXPECTED_ERRORS)) {
            fs::create_directories(EXPECTED_ERRORS);
        }
    }
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
    sim = loadFromFile("05_vehicle_ok.xml");
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

// NEW ERROR COMPARISON TESTS

// Test for basic invalid XML
TEST_F(TrafficSimulationErrorTest, BasicBadXmlErrors) {
    try {
        std::string actualErrors = captureAllOutput("01_basic_bad.xml");
        std::string expectedErrors = loadExpectedErrors("01_basic_bad");
        
        if (!expectedErrors.empty()) {
            EXPECT_TRUE(compareErrorStreams(actualErrors, expectedErrors)) 
                << "Error output does not match expected errors for 01_basic_bad.xml";
        } else {
            // If no expected errors file exists, just check that there are some errors
            EXPECT_FALSE(actualErrors.empty()) 
                << "Expected errors for invalid XML but got none";
        }
    } catch (const std::exception& e) {
        FAIL() << "Test crashed with exception: " << e.what();
    } catch (...) {
        FAIL() << "Test crashed with unknown exception";
    }
}

// Test for traffic light validation errors
TEST_F(TrafficSimulationErrorTest, TrafficLightBadErrors) {
    try {
        std::string actualErrors = captureAllOutput("03_tlight_bad.xml");
        std::string expectedErrors = loadExpectedErrors("03_tlight_bad");
        
        if (!expectedErrors.empty()) {
            EXPECT_TRUE(compareErrorStreams(actualErrors, expectedErrors)) 
                << "Error output does not match expected errors for 03_tlight_bad.xml";
        } else {
            EXPECT_FALSE(actualErrors.empty()) 
                << "Expected errors for invalid traffic light but got none";
        }
    } catch (const std::exception& e) {
        FAIL() << "Test crashed with exception: " << e.what();
    } catch (...) {
        FAIL() << "Test crashed with unknown exception";
    }
}

// Test for traffic light cycle validation errors
TEST_F(TrafficSimulationErrorTest, TrafficLightCycleBadErrors) {
    try {
        std::string actualErrors = captureAllOutput("04_tlightcycle_bad.xml");
        std::string expectedErrors = loadExpectedErrors("04_tlightcycle_bad");
        
        if (!expectedErrors.empty()) {
            EXPECT_TRUE(compareErrorStreams(actualErrors, expectedErrors)) 
                << "Error output does not match expected errors for 04_tlightcycle_bad.xml";
        } else {
            EXPECT_FALSE(actualErrors.empty()) 
                << "Expected errors for invalid traffic light cycle but got none";
        }
    } catch (const std::exception& e) {
        FAIL() << "Test crashed with exception: " << e.what();
    } catch (...) {
        FAIL() << "Test crashed with unknown exception";
    }
}

// Test for vehicle validation errors
TEST_F(TrafficSimulationErrorTest, VehicleBadErrors) {
    try {
        std::string actualErrors = captureAllOutput("05_vehicle_bad.xml");
        std::string expectedErrors = loadExpectedErrors("05_vehicle_bad");
        
        if (!expectedErrors.empty()) {
            EXPECT_TRUE(compareErrorStreams(actualErrors, expectedErrors)) 
                << "Error output does not match expected errors for 05_vehicle_bad.xml";
        } else {
            EXPECT_FALSE(actualErrors.empty()) 
                << "Expected errors for invalid vehicle but got none";
        }
    } catch (const std::exception& e) {
        FAIL() << "Test crashed with exception: " << e.what();
    } catch (...) {
        FAIL() << "Test crashed with unknown exception";
    }
}

// Test for vehicle type validation errors
TEST_F(TrafficSimulationErrorTest, VehicleTypeBadErrors) {
    try {
        std::string actualErrors = captureAllOutput("06_vehicletype_bad.xml");
        std::string expectedErrors = loadExpectedErrors("06_vehicletype_bad");
        
        if (!expectedErrors.empty()) {
            EXPECT_TRUE(compareErrorStreams(actualErrors, expectedErrors)) 
                << "Error output does not match expected errors for 06_vehicletype_bad.xml";
        } else {
            EXPECT_FALSE(actualErrors.empty()) 
                << "Expected errors for invalid vehicle type but got none";
        }
    } catch (const std::exception& e) {
        FAIL() << "Test crashed with exception: " << e.what();
    } catch (...) {
        FAIL() << "Test crashed with unknown exception";
    }
}

// Test for vehicle generator validation errors
TEST_F(TrafficSimulationErrorTest, VehicleGeneratorBadErrors) {
    try {
        std::string actualErrors = captureAllOutput("07_vgenerator_bad.xml");
        std::string expectedErrors = loadExpectedErrors("07_vgenerator_bad");
        
        if (!expectedErrors.empty()) {
            EXPECT_TRUE(compareErrorStreams(actualErrors, expectedErrors)) 
                << "Error output does not match expected errors for 07_vgenerator_bad.xml";
        } else {
            EXPECT_FALSE(actualErrors.empty()) 
                << "Expected errors for invalid vehicle generator but got none";
        }
    } catch (const std::exception& e) {
        FAIL() << "Test crashed with exception: " << e.what();
    } catch (...) {
        FAIL() << "Test crashed with unknown exception";
    }
}

// Test for bus stop validation errors
TEST_F(TrafficSimulationErrorTest, BusStopBadErrors) {
    try {
        std::string actualErrors = captureAllOutput("08_busstop_bad.xml");
        std::string expectedErrors = loadExpectedErrors("08_busstop_bad");
        
        if (!expectedErrors.empty()) {
            EXPECT_TRUE(compareErrorStreams(actualErrors, expectedErrors)) 
                << "Error output does not match expected errors for 08_busstop_bad.xml";
        } else {
            EXPECT_FALSE(actualErrors.empty()) 
                << "Expected errors for invalid bus stop but got none";
        }
    } catch (const std::exception& e) {
        FAIL() << "Test crashed with exception: " << e.what();
    } catch (...) {
        FAIL() << "Test crashed with unknown exception";
    }
}

// Test for intersection validation errors
TEST_F(TrafficSimulationErrorTest, IntersectionBadErrors) {
    try {
        std::string actualErrors = captureAllOutput("10_intersection_bad.xml");
        std::string expectedErrors = loadExpectedErrors("10_intersection_bad");
        
        if (!expectedErrors.empty()) {
            EXPECT_TRUE(compareErrorStreams(actualErrors, expectedErrors)) 
                << "Error output does not match expected errors for 10_intersection_bad.xml";
        } else {
            EXPECT_FALSE(actualErrors.empty()) 
                << "Expected errors for invalid intersection but got none";
        }
    } catch (const std::exception& e) {
        FAIL() << "Test crashed with exception: " << e.what();
    } catch (...) {
        FAIL() << "Test crashed with unknown exception";
    }
}

// NEW STRUCTURED ERROR COMPARISON TESTS USING COMPARE2

class XMLParseInValid : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure the directories exist
        if (!fs::exists(EXPECTED_ERRORS)) {
            fs::create_directories(EXPECTED_ERRORS);
        }
        if (!fs::exists(XML_TESTFILES)) {
            fs::create_directories(XML_TESTFILES);
        }
    }
};

// Simple test to verify file paths are working
TEST_F(XMLParseInValid, FilePathTest) {
    // Check if expected file exists
    EXPECT_TRUE(fs::exists("../tests/Expected_Testfiles/expected_01_basic_bad.txt")) 
        << "Expected file does not exist";
    
    // Check if XML file exists
    EXPECT_TRUE(fs::exists("../tests/XML_Testfiles/01_basic_bad.xml")) 
        << "XML file does not exist";
}

// Test to verify that the compare2 function can read files
TEST_F(XMLParseInValid, Compare2FileReadTest) {
    // Test if we can read the expected file directly
    std::ifstream testFile("../tests/Expected_Testfiles/expected_01_basic_bad.txt");
    EXPECT_TRUE(testFile.is_open()) << "Cannot open expected file for reading";
    if (testFile.is_open()) {
        std::string content;
        std::getline(testFile, content);
        EXPECT_FALSE(content.empty()) << "Expected file is empty";
        testFile.close();
    }
}

// Smart test function that analyzes XML and automatically determines expected behavior
static void smartTest(const std::string& xmlFile) {
    // Convert relative paths to absolute WSL paths
    std::string xmlPath = xmlFile;
    if (xmlFile.find("test_files/") == 0) {
        xmlPath = "../tests/" + xmlFile;
    }
    
    // Read and analyze the XML file
    std::ifstream xmlStream(xmlPath);
    if (!xmlStream.is_open()) {
        FAIL() << "Could not open XML file: " << xmlPath;
    }
    
    std::stringstream xmlBuffer;
    xmlBuffer << xmlStream.rdbuf();
    std::string xmlContent = xmlBuffer.str();
    
    // Analyze XML content to determine if errors are expected
    bool expectError = false;
    std::string expectedErrorType = "";
    
    // Check for common error patterns in XML
    if (xmlContent.find("cyclus=\"-") != std::string::npos) {
        expectError = true;
        expectedErrorType = "negative_cycle";
    }
    if (xmlContent.find("lengte=\"-") != std::string::npos) {
        expectError = true;
        expectedErrorType = "negative_length";
    }
    if (xmlContent.find("positie=\"-") != std::string::npos) {
        expectError = true;
        expectedErrorType = "negative_position";
    }
    if (xmlContent.find("cyclus=\"0\"") != std::string::npos) {
        expectError = true;
        expectedErrorType = "zero_cycle";
    }
    if (xmlContent.find("lengte=\"0\"") != std::string::npos) {
        expectError = true;
        expectedErrorType = "zero_length";
    }
    if (xmlContent.find("type=\"invalid\"") != std::string::npos) {
        expectError = true;
        expectedErrorType = "invalid_vehicle_type";
    }
    
    // Capture actual output including REQUIRE violations
    std::string actualOutput = captureOutputWithRequireViolations(xmlPath);
    
    // Determine expected output based on analysis
    std::string expectedOutput;
    if (expectError) {
        if (expectedErrorType == "negative_cycle") {
            expectedOutput = "REQUIRE Violation: cycle time must be positive in TrafficLight.cpp:25\n";
        } else if (expectedErrorType == "negative_length") {
            expectedOutput = "REQUIRE Violation: length must be positive in Road.cpp:XX\n";
        } else if (expectedErrorType == "negative_position") {
            expectedOutput = "REQUIRE Violation: position must be non-negative in Vehicle.cpp:XX\n";
        } else if (expectedErrorType == "zero_cycle") {
            expectedOutput = "REQUIRE Violation: cycle time must be positive in TrafficLight.cpp:25\n";
        } else if (expectedErrorType == "zero_length") {
            expectedOutput = "REQUIRE Violation: length must be positive in Road.cpp:XX\n";
        } else if (expectedErrorType == "invalid_vehicle_type") {
            expectedOutput = "REQUIRE Violation: invalid vehicle type in Vehicle.cpp:XX\n";
        } else {
            expectedOutput = "REQUIRE Violation: validation error\n";
        }
    } else {
        // If no errors expected, expect normal simulation output
        expectedOutput = "Increment: 1\nTijd: 0.0166\n\n";
        // Add more expected output patterns for successful simulation
    }
    
    // Normalize both outputs
    std::string normalizedActualOutput = normalizeOutput(actualOutput);
    std::string normalizedExpectedOutput = normalizeOutput(expectedOutput);
    
    // Compare the outputs
    if (expectError) {
        // For error cases, check if the actual output contains the expected error
        EXPECT_TRUE(actualOutput.find("REQUIRE Violation") != std::string::npos)
            << "Expected REQUIRE violation but got: " << actualOutput;
    } else {
        // For success cases, check if simulation runs without errors
        EXPECT_TRUE(actualOutput.find("REQUIRE Violation") == std::string::npos)
            << "Unexpected REQUIRE violation: " << actualOutput;
    }
    
    // Also compare the full outputs for detailed matching
    EXPECT_EQ(normalizedActualOutput, normalizedExpectedOutput)
        << "XML Analysis: " << (expectError ? "ERROR expected" : "SUCCESS expected")
        << " (Type: " << expectedErrorType << ")\n"
        << "Actual output:\n" << actualOutput
        << "\nExpected output:\n" << expectedOutput;
}

// Function to dynamically modify XML files for testing
static void modifyXMLFile(const std::string& xmlFile, const std::string& searchPattern, const std::string& replacement) {
    std::string xmlPath = xmlFile;
    if (xmlFile.find("XML_Testfiles/") == 0) {
        xmlPath = "../tests/" + xmlFile;
    }
    
    // Read the XML file
    std::ifstream inFile(xmlPath);
    if (!inFile.is_open()) {
        FAIL() << "Could not open XML file for modification: " << xmlPath;
    }
    
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string content = buffer.str();
    inFile.close();
    
    // Replace the pattern
    size_t pos = content.find(searchPattern);
    if (pos != std::string::npos) {
        content.replace(pos, searchPattern.length(), replacement);
    }
    
    // Write back to file
    std::ofstream outFile(xmlPath);
    if (!outFile.is_open()) {
        FAIL() << "Could not write to XML file: " << xmlPath;
    }
    outFile << content;
    outFile.close();
}