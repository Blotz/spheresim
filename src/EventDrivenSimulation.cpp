#include <cmath>
#include <queue>
#include <random>
#include <vector>

#include "EventDrivenSimulation.h"
#include "SpatialGrid.h"
#include "config.h"

EventDrivenSimulation::EventDrivenSimulation(int n)
    : current_time(0.0), collision_times{}, event_queue{} {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::poisson_distribution<int> poisson_dist(n);

  // Generate a random number from the Poisson distribution
  this->sphere_count = poisson_dist(gen);
  // epsilon is defined as length^d / n^(1 / d-1)
  double epsilon =
      pow(TORUS_SIZE, DIMENSIONS) / pow(n, 1.0 / (double)(DIMENSIONS - 1));
  double radius = epsilon * 0.5;

  int num_cells = floor(TORUS_SIZE / epsilon);
  long double cell_size = TORUS_SIZE / (long double)num_cells;

  // dt * v < epsilon -> dt < epsilon / v
  double velocity_magnitude = 1.0;
  this->max_dt = epsilon / velocity_magnitude;
  

  this->grid = SpatialGrid(cell_size, num_cells, this->sphere_count, radius);
}

void EventDrivenSimulation::run_simulation() {
  while (this->current_time < MAX_SIMULATION_TIME) {
    if (this->event_queue.empty()) {
      // no more events within max_dt
      // move all spheres to their next position
      this->grid.update_positions(this->max_dt);
      this->initialize_events();
      this->current_time += this->max_dt;
    } else {
      this->run_simulation_step();
    }
  }
}

void EventDrivenSimulation::run_simulation_step() {
  Event event = this->event_queue.top();
  this->event_queue.pop();

  this->current_time = event.time;
  this->handle_event(event);
}

void EventDrivenSimulation::handle_event(Event &event) {
  this->grid.update_positions(event.time - this->current_time);

  Sphere *s1 = event.s1;
  Sphere *s2 = event.s2;

  // TODO: handle edge case where spheres collide at boundary
  // discard event if spheres are no longer colliding
  // maybe offload this to the spatial grid
  double collision_time = collide(s1, s2);
  if (collision_time < 0) {
    return;
  }

  vec3 new_velocity1 = s1->collision_velocity(s2);
  vec3 new_velocity2 = s2->collision_velocity(s1);

  s1->set_velocity(new_velocity1);
  s2->set_velocity(new_velocity2);

  s1->decrement_collision_checks();
  s2->decrement_collision_checks();

  if (s1->get_max_collision_checks() > 0) {
    this->find_collision_events(s1);
  }

  if (s2->get_max_collision_checks() > 0) {
    this->find_collision_events(s2);
  }
}

void EventDrivenSimulation::initialize_events() {
  Sphere *spheres = this->grid.get_spheres();
  for (int i = 0; i < this->sphere_count; i++) {
    this->find_collision_events(&spheres[i]);
  }
}

void EventDrivenSimulation::find_collision_events(Sphere *s) {
  std::vector<Sphere *> nearby_spheres = this->grid.get_nearby_spheres(s);
  for (auto &other : nearby_spheres) {
    if (s == other) {
      continue;
    }

    double collision_time = collide(s, other);
    // discard event if spheres do not collide
    // discard event if collision time is greater than max_dt
    if (collision_time >= 0 && collision_time < this->max_dt) {
      this->event_queue.push(
          Event(this->current_time + collision_time, s, other));
    }
  }
}