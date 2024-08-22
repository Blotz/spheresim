#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>

#include "Sphere.h"
#include "vec3.h"

TEST_CASE("Sphere Constructor", "[Sphere]") {
  Sphere s1;
  Sphere s2(1, point3(0, 0, 0), vec3(1, 0, 0));
}

TEST_CASE("Sphere Equals") {
  Sphere s1(1, point3(0, 0, 0), vec3(1, 0, 0));
  Sphere s2(1, point3(0, 0, 0), vec3(1, 0, 0));
  REQUIRE(s1 == s1);
  REQUIRE(s2 == s2);
  REQUIRE(!(s1 == s2));
}

TEST_CASE("Sphere Collide", "[Sphere]") {
  // colliding Sphere
  Sphere s3(1, point3(0, 0, 0), vec3(1, 0, 0));
  Sphere s4(1, point3(3, 0, 0), vec3(-1, 0, 0));
  REQUIRE(collide(&s3, &s4) == 0.5);

  // touching Sphere
  Sphere s1(1, point3(0, 0, 0), vec3(1, 0, 0));
  Sphere s2(1, point3(2, 0, 0), vec3(-1, 0, 0));
  REQUIRE(collide(&s1, &s2) == 0.0);

  // non-colliding Sphere
  Sphere s5(1, point3(0, 0, 0), vec3(1, 0, 0));
  Sphere s6(1, point3(0, 3, 0), vec3(-1, 0, 0));
  REQUIRE(collide(&s5, &s6) == -1.0);

  // intersecting Sphere
  Sphere s7(1, point3(0, 0, 0), vec3(1, 0, 0));
  Sphere s8(1, point3(1, 0, 0), vec3(-1, 0, 0));
  REQUIRE(collide(&s7, &s8) == -1.0);
}

TEST_CASE("Sphere Update Position", "[Sphere]") {
  Sphere s1(1, point3(0, 0, 0), vec3(1, 0, 0));
  s1.update_position(1);
  REQUIRE(s1.get_center() == point3(1, 0, 0));

  Sphere s2(1, point3(0, 0, 0), vec3(1, 0, 0));
  Sphere s3(1, point3(2, 0, 0), vec3(-1, 0, 0));
  double t = collide(&s2, &s3);
  s2.update_position(t);
  s3.update_position(t);
  REQUIRE(collide(&s2, &s3) == 0.0);
}

TEST_CASE("Sphere Collision Velocity", "[Sphere]") {
  // moving towards each other
  Sphere s1(1, point3(0, 0, 0), vec3(1, 0, 0));
  Sphere s2(1, point3(2, 0, 0), vec3(-1, 0, 0));

  vec3 v1 = s1.collision_velocity(&s2);
  REQUIRE(v1 == vec3(-1, 0, 0));
  vec3 v2 = s2.collision_velocity(&s1);
  REQUIRE(v2 == vec3(1, 0, 0));

  // one stationary
  Sphere s3(1, point3(0, 0, 0), vec3(1, 0, 0));
  Sphere s4(1, point3(2, 0, 0), vec3(0, 0, 0));

  vec3 v3 = s3.collision_velocity(&s4);
  REQUIRE(v3 == vec3(0, 0, 0));
  vec3 v4 = s4.collision_velocity(&s3);
  REQUIRE(v4 == vec3(1, 0, 0));

  // two stationary
  Sphere s5(1, point3(0, 0, 0), vec3(0, 0, 0));
  Sphere s6(1, point3(2, 0, 0), vec3(0, 0, 0));

  vec3 v5 = s5.collision_velocity(&s6);
  REQUIRE(v5 == vec3(0, 0, 0));
  vec3 v6 = s6.collision_velocity(&s5);
  REQUIRE(v6 == vec3(0, 0, 0));
}