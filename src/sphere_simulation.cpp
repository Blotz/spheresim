#include "sphere_simulation.h"
#include "mod.h"
#include "vec3.h"
#include <limits>
#include <random>

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

  // data structures
  this->collision_times = std::vector<double>();
  this->event_queue = std::priority_queue<Event>();

  this->epsilon =
      1 / pow(n, (1 / (double)(this->dimensions - 1))); // diameter of a cell
  this->epsilon = epsilon / 2;                          // radius of a cell

  // checks to see if values are within the bounds of the simulation
  if (this->dimensions < 2) {
    throw std::invalid_argument("dimensions must be greater than 1");
  }
  // times 2 because the radius of the sphere is half the diameter
  // if (1 / (double)this->num_cells < this->epsilon * 2) {
  //   throw std::invalid_argument("Cells are smaller than the particles.");
  // }

  // Initialize the spheres
  this->spheres = new sphere[this->number_of_spheres];

  if (this->spheres == nullptr) {
    throw std::invalid_argument("Could not initialize spheres.");
  }

  // Generate n spheres
  for (int i = 0; i < this->number_of_spheres; i++) {
    point3 center(uniform_dist(gen), uniform_dist(gen), uniform_dist(gen));
    vec3 velocity(normal_dist(gen), normal_dist(gen), normal_dist(gen));

    velocity = unit_vector(velocity);
    sphere *s =
        new sphere(epsilon, center, velocity, this->max_collision_checks);
    this->spheres[i] = *s;
  }
}

sphere_simulation::~sphere_simulation() {
  if (this->spheres != nullptr) {
    delete[] this->spheres;
  }
}

void sphere_simulation::translate_into_bounds(sphere *s) {
  point3 pos = s->get_center();

  // this code is 2s slower
  // pos[0] = mod(pos[0], 1.0);
  // pos[1] = mod(pos[1], 1.0);
  // pos[2] = mod(pos[2], 1.0);

  bool has_changed = false;

  if (pos[0] < 0.0) {
    pos[0] += 1.0;
    has_changed = true;
  } else if (pos[0] >= 1.0) {
    pos[0] -= 1.0;
    has_changed = true;
  }

  if (pos[1] < 0.0) {
    pos[1] += 1.0;
    has_changed = true;
  } else if (pos[1] >= 1.0) {
    pos[1] -= 1.0;
    has_changed = true;
  }

  if (pos[2] < 0.0) {
    pos[2] += 1.0;
    has_changed = true;
  } else if (pos[2] >= 1.0) {
    pos[2] -= 1.0;
    has_changed = true;
  }

  if (has_changed) {
    s->set_position(pos);
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

void sphere_simulation::initialize_events() {
  for (int i = 0; i < number_of_spheres; i++) {
    for (int j = i + 1; j < number_of_spheres; j++) {
      add_possible_collision_event(&spheres[i], &spheres[j]);
    }
  }
}

void sphere_simulation::handle_event(const Event &event) {
  // // Add new events for the affected spheres
  // for (int i = 0; i < number_of_spheres; i++) {
  //   if (event.s1 != &spheres[i]) {
  //     add_possible_collision_event(event.s1, &spheres[i]);
  //   }
  //   if (event.s2 != &spheres[i]) {
  //     add_possible_collision_event(event.s2, &spheres[i]);
  //   }
  // }

  // Check if spheres still collide at this time
  double collision_time = event.s1->collide(event.s2);

  if (collision_time < 0) {
    return;
  }
  // Move to the time of the event
  update_positions(event.time - current_time);
  current_time = event.time;
  std::cout << "time: " << current_time << std::endl;

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

  // Add new events for the affected spheres
  for (int i = 0; i < number_of_spheres; i++) {
    if (event.s1 != &spheres[i]) {
      add_collision_event(event.s1, &spheres[i]);
    }
    if (event.s2 != &spheres[i]) {
      add_collision_event(event.s2, &spheres[i]);
    }
  }
}

void sphere_simulation::update_positions(double dt) {
  for (int i = 0; i < number_of_spheres; i++) {
    this->spheres[i].update_position(dt);
    translate_into_bounds(&this->spheres[i]);
  }
}

void sphere_simulation::add_possible_collision_event(sphere *s1, sphere *s2) {
  point3 position = s1->get_center();
  vec3 velocity = s1->get_velocity();
  double dt = this->max_time - this->current_time;

  // check periodic positions
  add_collision_event(s1, s2);

  point3 possible_position = position + velocity * dt;

  if (possible_position[0] < 0.0) {
    s1->set_position(position + vec3(1.0, 0.0, 0.0));
    add_collision_event(s1, s2);
  }

  if (possible_position[1] < 0.0) {
    s1->set_position(position + vec3(0.0, 1.0, 0.0));
    add_collision_event(s1, s2);
  }

  if (possible_position[2] < 0.0) {
    s1->set_position(position + vec3(0.0, 0.0, 1.0));
    add_collision_event(s1, s2);
  }

  if (possible_position[0] >= 1.0) {
    s1->set_position(position - vec3(1.0, 0.0, 0.0));
    add_collision_event(s1, s2);
  }

  if (possible_position[1] >= 1.0) {
    s1->set_position(position - vec3(0.0, 1.0, 0.0));
    add_collision_event(s1, s2);
  }

  if (possible_position[2] >= 1.0) {
    s1->set_position(position - vec3(0.0, 0.0, 1.0));
    add_collision_event(s1, s2);
  }

  s1->set_position(position);
}

void sphere_simulation::add_possible_collision_event(sphere *s) {
  for (int i = 0; i < number_of_spheres; i++) {
    if (s != &spheres[i]) {
      add_possible_collision_event(s, &spheres[i]);
    }
  }
}

void sphere_simulation::add_collision_event(sphere *s1, sphere *s2) {
  double collision_time = s1->collide(s2);

  if (collision_time >= 0 &&
      collision_time + this->current_time <= this->max_time) {
    this->event_queue.push(Event(current_time + collision_time, s1, s2));
  }
}
