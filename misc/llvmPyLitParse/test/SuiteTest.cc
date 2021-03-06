#include <catch2/catch.hpp>
#include <llvmPy/Lit.h>
#include <llvmPy/LitParser.h>
#include <build.h>
#include <sstream>
#include <vector>
using namespace llvmPy;

TEST_CASE("Parsing output from a lit suite") {
    std::vector<LitTestResult *> results;
    int status = lit(results, TEST_PATH);
    CHECK(status != 0);
    REQUIRE(results.size() == 2);
    // TODO: The ordering is probably dependent on arbitrary factors.
    REQUIRE(results[1]->getResultCode() == LitResultCode::PASS);
    REQUIRE(results[0]->getResultCode() == LitResultCode::FAIL);
}
