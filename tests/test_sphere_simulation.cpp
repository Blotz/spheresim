#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>

#include "Sphere.h"
#include "sphere_simulation.h"


TEST_CASE("Sphere Sim Constructor") {
  sphere_simulation sim(10000);

  sphere_simulation sim2(10, new Sphere[10]);
}

TEST_CASE("Sphere Wrap Around Detection") {
  Sphere* spheres;

  // no wrap around
  spheres = new Sphere[2];
  spheres[0] = Sphere(0.05, point3(0.5, 0.5, 0.5), vec3(0, 0, 0));
  spheres[1] = Sphere(0.05, point3(0.5, 0.5, 0.5), vec3(0, 0, 0));

  sphere_simulation sim1(2, spheres);
  sim1.initialize_events();
  sim1.run_simulation();

  REQUIRE(sim1.get_collision_times().size() == 0);

  REQUIRE(spheres[0].get_center().isApprox(point3(0.5, 0.5, 0.5)));
  REQUIRE(spheres[1].get_center().isApprox(point3(0.5, 0.5, 0.5)));

  REQUIRE(spheres[0].get_velocity().isApprox(vec3(0, 0, 0)));
  REQUIRE(spheres[1].get_velocity().isApprox(vec3(0, 0, 0)));

  // positive wrap around
  spheres = new Sphere[2];
  spheres[0] = Sphere(0.05, point3(0.75, 0.5, 0.5), vec3(1, 0, 0));
  spheres[1] = Sphere(0.05, point3(0.5, 0.5, 0.5), vec3(0, 0, 0));

  sphere_simulation sim2(2, spheres);
  sim2.initialize_events();
  sim2.run_simulation();

  REQUIRE(sim2.get_collision_times().size() == 1);

  REQUIRE(spheres[0].get_center().isApprox(point3(0.4, 0.5, 0.5)));
  REQUIRE(spheres[1].get_center().isApprox(point3(0.85, 0.5, 0.5)));

  REQUIRE(spheres[0].get_velocity().isApprox(vec3(0, 0, 0)));
  REQUIRE(spheres[1].get_velocity().isApprox(vec3(1, 0, 0)));

  // negative wrap around
  spheres = new Sphere[2];
  spheres[0] = Sphere(0.05, point3(0.25, 0.5, 0.5), vec3(-1, 0, 0));
  spheres[1] = Sphere(0.05, point3(0.5, 0.5, 0.5), vec3(0, 0, 0));

  sphere_simulation sim3(2, spheres);
  sim3.initialize_events();
  sim3.run_simulation();

  REQUIRE(sim3.get_collision_times().size() == 1);

  REQUIRE(spheres[0].get_center().isApprox(point3(0.6, 0.5, 0.5)));
  REQUIRE(spheres[1].get_center().isApprox(point3(0.15, 0.5, 0.5)));

  REQUIRE(spheres[0].get_velocity().isApprox(vec3(0, 0, 0)));
  REQUIRE(spheres[1].get_velocity().isApprox(vec3(-1, 0, 0)));

}

TEST_CASE("Sphere Sim Collision Detection") {
  Sphere* spheres;

  // simple collision
  spheres = new Sphere[2];
  spheres[0] = Sphere(0.05, point3(0.0, 0.5, 0.5), vec3(2.0, 0, 0));
  spheres[1] = Sphere(0.05, point3(1.0, 0.5, 0.5), vec3(-2.0, 0, 0));

  // passes if collision detected
  REQUIRE(collide(&spheres[0], &spheres[1]) >= 0.0);

  sphere_simulation sim1(2, spheres);

  sim1.initialize_events();
  sim1.run_simulation();

  REQUIRE(sim1.get_collision_times().size() == 1);

  // no collision
  spheres = new Sphere[2];
  spheres[0] = Sphere(0.05, point3(0.0, 0.5, 0.5), vec3(2.0, 0, 0));
  spheres[1] = Sphere(0.05, point3(1.0, 0.5, 0.5), vec3(2.0, 0, 0));

  // passes if no collision detected
  REQUIRE(collide(&spheres[0], &spheres[1]) == -1.0);

  sphere_simulation sim2(2, spheres);

  sim2.initialize_events();
  sim2.run_simulation();

  REQUIRE(sim2.get_collision_times().size() == 0);


  // wrap around collision
  spheres = new Sphere[2];
  spheres[0] = Sphere(0.05, point3(0.3, 0.5, 0.5), vec3(-1.0, 0, 0));
  spheres[1] = Sphere(0.05, point3(0.5, 0.5, 0.5), vec3(0, 0, 0));

  sphere_simulation sim3(2, spheres);

  sim3.initialize_events();
  sim3.run_simulation();

  REQUIRE(sim3.get_collision_times().size() == 1); 

  // collisions at boundaries without intersecting the boundary
  spheres = new Sphere[2];
  spheres[0] = Sphere(0.05, point3(0.02, 0.2, 0.5), vec3(0, 1, 0));
  spheres[1] = Sphere(0.05, point3(0.98, 0.8, 0.5), vec3(0, -1, 0));

  sphere_simulation sim5(2, spheres);

  sim5.initialize_events();
  sim5.run_simulation();

  REQUIRE(sim5.get_collision_times().size() == 1);

  // collisions at boundaries with intersecting the boundary
  spheres = new Sphere[2];
  spheres[0] = Sphere(0.05, point3(0.02, 0.5, 0.5), vec3(0, 0, 0));
  spheres[1] = Sphere(0.05, point3(0.5, 0.5, 0.5), vec3(1, 0, 0));

  sphere_simulation sim4(2, spheres);

  sim4.initialize_events();
  sim4.run_simulation();

  REQUIRE(sim4.get_collision_times().size() == 1);
}

TEST_CASE("Sphere Sim Billiards Collision Detection") {
  Sphere* spheres;

  // simple collision
  spheres = new Sphere[3];
  spheres[0] = Sphere(0.05, point3(0.1, 0.5, 0.5), vec3(2.0, 0, 0));
  spheres[1] = Sphere(0.05, point3(0.5, 0.5, 0.5), vec3(0, 0, 0));
  spheres[2] = Sphere(0.05, point3(1.0, 0.5, 0.5), vec3(-2.0, 0, 0));

  sphere_simulation sim1(3, spheres);

  sim1.initialize_events();
  sim1.run_simulation();

  REQUIRE(sim1.get_collision_times().size() == 1);

  REQUIRE(spheres[0].get_max_collision_checks() == 0);
  REQUIRE(spheres[1].get_max_collision_checks() == 0);
  REQUIRE(spheres[2].get_max_collision_checks() == 1);

  REQUIRE(spheres[0].get_center().isApprox(point3(0.4, 0.5, 0.5)));
  REQUIRE(spheres[1].get_center().isApprox(point3(0.2, 0.5, 0.5)));
  REQUIRE(spheres[2].get_center().isApprox(point3(0.0, 0.5, 0.5)));

  REQUIRE(spheres[0].get_velocity().isApprox(vec3(0, 0, 0)));
  REQUIRE(spheres[1].get_velocity().isApprox(vec3(2, 0, 0)));
  REQUIRE(spheres[2].get_velocity().isApprox(vec3(-2, 0, 0)));


}