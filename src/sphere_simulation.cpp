#include "sphere_simulation.h"
#include "mod.h"
#include "vec3.h"
#include "config.h"

#include <limits>
#include <random>
#include <vector>

sphere_simulation::sphere_simulation(int n) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::poisson_distribution<int> poisson_dist(n);
  std::normal_distribution<double> normal_dist(0, 1);
  std::uniform_real_distribution<double> uniform_dist(0, 1);

  // Generate a random number from the Poisson distribution
  this->number_of_spheres = poisson_dist(gen);
  this->dimensions = 3;
  this->max_collision_checks = 1;
  this->max_time = 1.0;
  this->current_time = 0.0;
  this->torus_size = 1.0;

  // data structures
  this->collision_times = std::vector<double>();
  this->event_queue = std::priority_queue<Event>();
  this->spheres = new sphere[this->number_of_spheres];

  if (this->spheres == nullptr) {
    throw std::invalid_argument("Could not initialize spheres.");
  }

  this->epsilon = pow(this->torus_size, this->dimensions) / pow(n, 1/((double) this->dimensions - 1));

  // Generate n spheres
  for (int i = 0; i < this->number_of_spheres; i++) {
    point3 center(uniform_dist(gen), uniform_dist(gen), uniform_dist(gen));
    vec3 velocity(normal_dist(gen), normal_dist(gen), normal_dist(gen));

    velocity = unit_vector(velocity);
    velocity *= 2.0;

    sphere *s = new sphere(
      epsilon / 2.0, 
      center, 
      velocity, 
      this->max_collision_checks, 
      this->torus_size
      );
    this->spheres[i] = *s;
  }
}
sphere_simulation::sphere_simulation(int n, sphere* spheres) {
  this->max_time = 1.0;
  this->current_time = 0.0;
    // data structures
  this->collision_times = std::vector<double>();
  this->event_queue = std::priority_queue<Event>();
  this->spheres = new sphere[n];

  for (int i = 0; i < n; i++) {
    this->spheres[n] = spheres[n];
  }
  
}


sphere_simulation::~sphere_simulation() {
  if (this->spheres != nullptr) {
    delete[] this->spheres;
  }
}

void sphere_simulation::initialize_events() {
  for (int i = 0; i < number_of_spheres; i++) {

    std::vector<point3> images = get_images(&spheres[i]);
    for (int j = 0; j < images.size(); j++) {
      spheres[i].set_position(images[j]);

      for (int k = i + 1; k < number_of_spheres; k++) {
        add_collision_event(&spheres[i], &spheres[k]);
      }
    }
  }
}

void sphere_simulation::run_simulation() {
  while (!event_queue.empty() && current_time < max_time) {
    run_simulation_step();
  }
}

void sphere_simulation::run_simulation_step() {
  Event event = event_queue.top();
  event_queue.pop();

  // Handle the event
  handle_event(event);
}

void sphere_simulation::handle_event(const Event &event) {
  // Check if spheres still collide at this time
  double collision_time = collide(event.s1, event.s2);

  if (collision_time < 0) {
    return; // invalidated by previous collision. discard event.
  }

  // Move to the time of the event
  update_positions(event.time - current_time);
  current_time = event.time;

  // std::cout << "time: " << current_time 
  //           << " queue: " << this->event_queue.size() << std::endl;

  // Update velocities
  vec3 v1 = event.s1->collision_velocity(*event.s2);
  vec3 v2 = event.s2->collision_velocity(*event.s1);
  event.s1->set_velocity(v1);
  event.s2->set_velocity(v2);

  // decrement collision checks
  event.s1->decrement_collision_checks();
  event.s2->decrement_collision_checks();

  // Add collision time to the vector
  this->collision_times.push_back(current_time + collision_time);

  this->find_collision_events(event.s1, event.s2);
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

void sphere_simulation::find_collision_events(sphere *s1, sphere *s2) {
  // // Add new events for the affected spheres
  std::vector<point3> s1_images = get_images(s1);
  std::vector<point3> s2_images = get_images(s2);
  size_t s1_images_size = s1_images.size();
  size_t s2_images_size = s2_images.size();
  size_t max_size = std::max(s1_images_size, s2_images_size);

  for (size_t i = 0; i < max_size; i++) {
    if (i < s1_images_size) {
      s1->set_position(s1_images[i]);
    }
    if (i < s2_images_size) {
      s2->set_position(s2_images[i]);
    }
    
    for (int j = 0; j < number_of_spheres; j++) {
      if (i < s1_images_size && s1 != &spheres[j]) {
        add_collision_event(s1, &spheres[j]);
      }
      if (i < s2_images_size && s2 != &spheres[j]) {
        add_collision_event(s2, &spheres[j]);
      }
    }
  }
  
}

std::vector<point3> sphere_simulation::get_images(sphere* s) {
  point3 center = s->get_center();
  point3 future_center = center + s->get_velocity() * (this->max_time - this->current_time);
  std::vector<point3> tarus_images = std::vector<point3>();
  
  if (future_center[0] > this->torus_size) {
    tarus_images.push_back(point3(-this->torus_size, 0, 0));
  } else if (future_center[0] < 0) {
    tarus_images.push_back(point3(this->torus_size, 0, 0));
  }
  if (future_center[1] > this->torus_size) {
    tarus_images.push_back(point3(0, -this->torus_size, 0));
  } else if (future_center[1] < 0) {
    tarus_images.push_back(point3(0, this->torus_size, 0));
  }
  if (future_center[2] > this->torus_size) {
    tarus_images.push_back(point3(0, 0, -this->torus_size));
  } else if (future_center[2] < 0) {
    tarus_images.push_back(point3(0, 0, this->torus_size));
  }

  tarus_images.push_back(center);

  return tarus_images;
}

void sphere_simulation::wrap_around(sphere* s) {
  point3 center = s->get_center();
  point3 future_center = center + s->get_velocity() * (this->max_time - this->current_time);

  for (int i = 0; i < this->dimensions; i++) {
    if (future_center[i] > this->torus_size) {
      center[i] -= this->torus_size;
    } else if (future_center[i] < 0) {
      center[i] += this->torus_size;
    }
  }
}