#include <limits>
#include <random>
#include <vector>

#include "config.h"
#include "sphere_simulation.h"
#include "vec3.h"

sphere_simulation::sphere_simulation(int n) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::poisson_distribution<int> poisson_dist(n);
  std::normal_distribution<double> normal_dist(0, 1);
  std::uniform_real_distribution<double> uniform_dist(0, 1);

  // Generate a random number from the Poisson distribution
  this->number_of_spheres = poisson_dist(gen);
  this->max_time = MAX_SIMULATION_TIME;
  this->current_time = 0.0;
  this->torus_size = 1.0;

  // data structures
  this->collision_times = std::vector<double>();
  this->event_queue = std::priority_queue<Event>();
  this->spheres = new sphere[this->number_of_spheres];

  if (this->spheres == nullptr) {
    throw std::invalid_argument("Could not initialize spheres.");
  }

  this->epsilon =
      pow(this->torus_size, DIMENSIONS) / pow(n, 1 / (double)(DIMENSIONS - 1));

  // Generate n spheres
  for (int i = 0; i < this->number_of_spheres; i++) {
    point3 center(uniform_dist(gen), uniform_dist(gen), uniform_dist(gen));
    vec3 velocity(normal_dist(gen), normal_dist(gen), normal_dist(gen));
    velocity.normalize();

    sphere *s =
        new sphere(epsilon / 2.0, center, velocity); // radius = epsilon/2
    this->spheres[i] = *s;
  }
}

sphere_simulation::sphere_simulation(int n, sphere *spheres) {
  this->max_time = MAX_SIMULATION_TIME;
  this->current_time = 0.0;
  this->torus_size = 1.0;
  // data structures
  this->collision_times = std::vector<double>();
  this->event_queue = std::priority_queue<Event>();
  this->spheres = spheres;
  this->number_of_spheres = n;
}

sphere_simulation::~sphere_simulation() {
  if (this->spheres != nullptr) {
    delete[] this->spheres;
    this->spheres = nullptr;
  }
}

void sphere_simulation::initialize_events() {
  for (int i = 0; i < number_of_spheres; i++) {
    find_collision_events(&spheres[i]);
  }
}

void sphere_simulation::run_simulation() {
  while (!event_queue.empty() && current_time < max_time) {
    run_simulation_step();
  }
  // if there are no more events, update the positions to the max time
  if (current_time < max_time) {
    double dt = max_time - current_time;
    update_positions(dt);
    
    for (int i = 0; i < number_of_spheres; i++) {
      wrap_around(&spheres[i]);
    }
    current_time = max_time;
  }

}

void sphere_simulation::run_simulation_step() {
  Event event = event_queue.top();
  event_queue.pop();

  handle_event(event);
}

void sphere_simulation::handle_event(Event &event) {
  // Move to the time of the event
  update_positions(event.time - current_time);
  current_time = event.time;

  // wrap_around(event.s1);
  // wrap_around(event.s2);

  // Check if spheres still collide at this time
  double collision_time = collide(event.s1, event.s2);

  if (collision_time < 0) {
    return; // invalidated by previous collision. discard event.
  }

  // Update velocities
  vec3 v1 = event.s1->collision_velocity(event.s2);
  vec3 v2 = event.s2->collision_velocity(event.s1);
  event.s1->set_velocity(v1);
  event.s2->set_velocity(v2);

  // decrement collision checks
  event.s1->decrement_collision_checks();
  event.s2->decrement_collision_checks();

  // Add collision time to the vector
  this->collision_times.push_back(current_time);

  this->find_collision_events(event.s1);
  this->find_collision_events(event.s2);
}

void sphere_simulation::update_positions(double dt) {
  for (int i = 0; i < number_of_spheres; i++) {
    this->spheres[i].update_position(dt);
    this->wrap_around(&this->spheres[i]);
  }
}

void sphere_simulation::add_collision_event(sphere *s1, sphere *s2) {
  double collision_time = collide(s1, s2);

  if (collision_time >= 0) {
    this->event_queue.push(Event(current_time + collision_time, s1, s2));
  }
}

void sphere_simulation::find_collision_events(sphere *s1) {
  // Add new events for the affected spheres
  std::vector<point3> s1_images = get_images(s1);

  for (int j = 0; j < number_of_spheres; j++) {
    if (s1 == &spheres[j]) {
      continue;
    }

    for (int k = 0; k < s1_images.size(); k++) {
      s1->set_position(s1_images[k]);
      add_collision_event(s1, &spheres[j]);
    }
  }
}

std::vector<point3> sphere_simulation::get_images(sphere *s) {
  point3 &center = s->get_center();
  point3 future_center =
      center + s->get_velocity() * (this->max_time - this->current_time);
  std::vector<point3> tarus_images = std::vector<point3>();

  for (int i = 0; i < DIMENSIONS; i++) {
    if (future_center[i] >= this->torus_size - this->epsilon) {
      point3 image = center;
      image[i] -= this->torus_size;
      tarus_images.push_back(image);
    } else if (future_center[i] + this->epsilon < 0) {
      point3 image = center;
      image[i] += this->torus_size;
      tarus_images.push_back(image);
    }
  }

  tarus_images.push_back(center);

  return tarus_images;
}

void sphere_simulation::wrap_around(sphere *s) {
  point3 &center = s->get_center();

  for (int i = 0; i < DIMENSIONS; i++) {
    if (center[i] >= this->torus_size) {
      center[i] -= this->torus_size;
    } else if (center[i] < 0) {
      center[i] += this->torus_size;
    }
  }
}