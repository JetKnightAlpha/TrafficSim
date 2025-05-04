cmake_minimum_required(VERSION 3.10)
project(TrafficSimulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Hoofdapplicatie ---
add_executable(TrafficSimulator
        src/main.cpp
        src/Simulation.cpp
        src/Road.cpp
        src/Vehicle.cpp
        src/TrafficLight.cpp
        src/VehicleGenerator.cpp
        src/Parser.cpp
        tinyxml/tinyxml.cpp
        tinyxml/tinystr.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxmlparser.cpp
        src/BusStop.cpp
        src/GraphicsEngine.cpp
        src/Intersection.cpp

)

target_include_directories(TrafficSimulator PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

# --- Testapplicatie ---
# 1. GTest headers en libraries
include_directories(${PROJECT_SOURCE_DIR}/gtest/include)
link_directories(${PROJECT_SOURCE_DIR}/gtest/lib)

# 2. Testexecutable
add_executable(TrafficSimulatorTests
        tests/tests.cpp
        src/Simulation.cpp
        src/Road.cpp
        src/Vehicle.cpp
        src/TrafficLight.cpp
        src/VehicleGenerator.cpp
        src/Parser.cpp
        tests/tests.cpp
        tinyxml/tinyxml.cpp
        tinyxml/tinystr.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxmlparser.cpp
        src/BusStop.cpp
        src/GraphicsEngine.cpp
        src/Intersection.cpp
)

target_include_directories(TrafficSimulatorTests PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

# 3. Linken naar GTest en pthread (of weglaten op Windows als je geen pthread hebt)
target_link_libraries(TrafficSimulatorTests
        gtest
        gtest_main
        pthread
)

# --- Gemakstarget om tests te runnen ---
add_custom_target(run_tests
        COMMAND TrafficSimulatorTests
        DEPENDS TrafficSimulatorTests
        COMMENT "Running tests..."
)
