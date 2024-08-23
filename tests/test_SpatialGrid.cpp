#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>

#include "SpatialGrid.h"

TEST_CASE("Spatial Grid Constructor") {
  SpatialGrid grid(0.1, 10, 10, 0.05);
}

TEST_CASE("Spatial Grid Add Sphere") {
  SpatialGrid grid(0.1, 10, 10, 0.05);
}