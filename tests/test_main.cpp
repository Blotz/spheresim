#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <Eigen/Dense>

TEST_CASE("Eigen vector test", "[eigen]") {
    Eigen::Vector3f vec(1.0f, 2.0f, 3.0f);
    REQUIRE(vec.x() == 1.0f);
    REQUIRE(vec.y() == 2.0f);
    REQUIRE(vec.z() == 3.0f);
}