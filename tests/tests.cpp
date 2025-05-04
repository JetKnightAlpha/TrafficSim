#include "gtest/gtest.h"
#include "Simulation.h"
#include <filesystem>

namespace fs = std::filesystem;
static const fs::path RES =
        fs::path(PROJECT_SOURCE_DIR) / "tests" / "test_files";

/* ------------------------------------------------------------------ */
/*  Hulproutine                                                       */
/* ------------------------------------------------------------------ */
static bool loadOK(const std::string& file)
{
    Simulation sim;
    return sim.loadFromFile( (RES / file).string() );
}

/* ------------------------------------------------------------------ */
/*  VALID  – moet slagen                                              */
/* ------------------------------------------------------------------ */
class ValidXML : public ::testing::TestWithParam<const char*> {};
TEST_P(ValidXML, ShouldLoad)
{
    EXPECT_TRUE( loadOK(GetParam()) );         // verwacht  TRUE  →  PASS
}
INSTANTIATE_TEST_CASE_P(ValidSuite, ValidXML, ::testing::Values(
        "01_basic_ok.xml",
        "02_roadlength_ok.xml",
        "03_tlight_ok.xml",
        "04_tlightcycle_ok.xml",
        "05_vehicle_ok.xml",
        "06_vehicletype_ok.xml",
        "07_vgenerator_ok.xml",
        "08_busstop_ok.xml",
        "09_busstopwait_ok.xml",
        "10_intersection_ok.xml"));

/* ------------------------------------------------------------------ */
/*  INVALID – moet falen (opzet)                                      */
/* ------------------------------------------------------------------ */
class InvalidXML : public ::testing::TestWithParam<const char*> {};
TEST_P(InvalidXML, ShouldFailButWeExpectSuccess)      // naam verduidelijkt
{
    /* We EXPECT TRUE → dit zal FALEN omdat loadOK()  false  teruggeeft.   */
    EXPECT_TRUE( loadOK(GetParam()) );         // veroorzaakt test‑FAIL
}
INSTANTIATE_TEST_CASE_P(InvalidSuite, InvalidXML, ::testing::Values(
        "01_basic_bad.xml",
        "02_roadlength_bad.xml",
        "03_tlight_bad.xml",
        "04_tlightcycle_bad.xml",
        "05_vehicle_bad.xml",
        "06_vehicletype_bad.xml",
        "07_vgenerator_bad.xml",
        "08_busstop_bad.xml",
        "09_busstopwait_bad.xml",
        "10_intersection_bad.xml"));
