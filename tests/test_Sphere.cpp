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

  resolve_collision(&s1, &s2);
  REQUIRE(s1.get_velocity() == vec3(-1, 0, 0));
  REQUIRE(s2.get_velocity() == vec3(1, 0, 0));

  // one stationary
  Sphere s3(1, point3(0, 0, 0), vec3(1, 0, 0));
  Sphere s4(1, point3(2, 0, 0), vec3(0, 0, 0));

  resolve_collision(&s3, &s4);
  REQUIRE(s3.get_velocity() == vec3(0, 0, 0));
  REQUIRE(s4.get_velocity() == vec3(1, 0, 0));

  // two stationary
  Sphere s5(1, point3(0, 0, 0), vec3(0, 0, 0));
  Sphere s6(1, point3(2, 0, 0), vec3(0, 0, 0));

  resolve_collision(&s5, &s6);
  REQUIRE(s5.get_velocity() == vec3(0, 0, 0));
  REQUIRE(s6.get_velocity() == vec3(0, 0, 0));
}