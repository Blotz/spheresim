#ifndef SPHERE_SIMULATION_H
#define SPHERE_SIMULATION_H

#include <queue>
#include <vector>

#include "sphere.h"

struct Event {
  double time;
  sphere *s1;
  sphere *s2;

  bool operator<(const Event &other) const {
    return time > other.time; // for min-heap
  }
  Event(double time, sphere *s1, sphere *s2) : time(time), s1(s1), s2(s2) {}
};

class sphere_simulation {
public:
  sphere_simulation(int n);
  sphere_simulation(int n, sphere* sphere);
  ~sphere_simulation();


  void run_simulation();
  void run_simulation_step();

  void initialize_events();

  void handle_event(const Event &event);
  void update_positions(double dt);

  void add_collision_event(sphere *s1, sphere *s2);
  void find_collision_events(sphere *s1, sphere *s2);

  void wrap_around(sphere* s);
  std::vector<point3> get_images(sphere* s);

  inline friend std::ostream &operator<<(std::ostream &out,
                                         const sphere_simulation &s) {
  out << "sphere_simulation { number_of_spheres: " << s.number_of_spheres
      << ", dimensions: " << s.dimensions 
      << ", epsilon: " << s.epsilon
      << ", max collisions: " << s.max_collision_checks
      << " }";
  return out;
  }

  sphere* get_spheres() const {
    return spheres;
  }
  int get_number_of_spheres() const {
    return number_of_spheres;
  }
  const std::vector<double> get_collision_times() const {
    return collision_times;
  }

private:
  std::priority_queue<Event> event_queue;
  sphere *spheres;

  double max_time;
  double current_time;
  int number_of_spheres;
  int max_collision_checks;
  int dimensions;
  double torus_size;
  double epsilon; // radius

  std::vector<double> collision_times; // stores the collision times for each sphere
};

#endif