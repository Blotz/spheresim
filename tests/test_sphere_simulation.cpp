#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>

#include "sphere.h"
#include "sphere_simulation.h"


TEST_CASE("Sphere Sim Constructor", "[sphere_sim]") {
  sphere_simulation sim(10000);

  sphere_simulation sim2(10, new sphere[10]);
}

TEST_CASE("Sphere Sim Collision Detection", "[sphere_sim]") {
  // simple collision
  sphere* spheres = new sphere[2];
  spheres[0] = sphere(0.05, point3(0.0, 0.5, 0.5), vec3(2.0, 0, 0));
  spheres[1] = sphere(0.05, point3(1.0, 0.5, 0.5), vec3(-2.0, 0, 0));

  sphere_simulation sim1(2, spheres);
  spheres = nullptr;

  sim1.initialize_events();
  sim1.run_simulation();

  REQUIRE(sim1.get_collision_times().size() == 1);
  // no collision
  // spheres = new sphere[2];
  // spheres[0] = sphere(0.05, point3(0.0, 0.5, 0.5), vec3(2.0, 0, 0));
  // spheres[1] = sphere(0.05, point3(1.0, 0.5, 0.5), vec3(2.0, 0, 0));

}