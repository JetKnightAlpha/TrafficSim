// tests/helpers.h -----------------------------------------------------------
#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <gtest/gtest.h>

inline std::string readFile(const std::string& path)
{
    std::ifstream f(path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// draait de simulatie en stuurt alle fouten naar errStream
inline void runSimulationCaptureErr(const std::string& xml,
                                    std::ostringstream& errStream)
{
    std::streambuf* oldErr = std::cerr.rdbuf(errStream.rdbuf());
    try {
        Simulation sim;
        sim.loadFromXml(xml);   // hier kunnen al fouten komen
        sim.run();              // en hier ook
    } catch (...) {
        // vangt uitzonderingen zodat ze ook in de stream terecht-
        // komen, niet alleen in Google-Test FAIL().
    }
    std::cerr.rdbuf(oldErr);
}

// Google-Test assertie: vergelijkt **exact** (dus extra regels → FAIL)
inline ::testing::AssertionResult
compareFiles(const std::string& expectedFile,
             const std::string& inputFile)
{
    std::string expected = readFile(expectedFile);

    std::ostringstream err;
    runSimulationCaptureErr(inputFile, err);
    std::string actual = err.str();

    if (expected == actual)
        return ::testing::AssertionSuccess();

    return ::testing::AssertionFailure()
           << "Expected-file: " << expectedFile << "\n"
           << "Input-file   : " << inputFile   << "\n\n"
           << "=== Expected ===\n" << expected
           << "===   Got    ===\n" << actual;
}
