#define CATCH_CONFIG_MAIN
#include <Eigen/Dense>
#include <catch2/catch_test_macros.hpp>

#include "config.h"
#include "vec3.h"

TEST_CASE("Eigen vector test", "[eigen]") {
  Eigen::Vector3f vec(1.0f, 2.0f, 3.0f);
  REQUIRE(vec.x() == 1.0f);
  REQUIRE(vec.y() == 2.0f);
  REQUIRE(vec.z() == 3.0f);

  point3 test = point3::Ones();
  point3 test2 = point3::Zero();

  test[0] = 1.0;


}