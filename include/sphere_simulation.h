#ifndef SPHERE_SIMULATION_H
#define SPHERE_SIMULATION_H

#include <queue>
#include <vector>

#include "Sphere.h"

struct Event {
  long double time;
  Sphere *s1;
  Sphere *s2;

  bool operator<(const Event &other) const {
    return time > other.time; // for min-heap
  }
  Event(long double time, Sphere *s1, Sphere *s2)
      : time(time), s1(s1), s2(s2) {}
};

class sphere_simulation {
public:
  sphere_simulation(int n);
  sphere_simulation(int n, Sphere *spheres);
  ~sphere_simulation();

  void run_simulation();
  void run_simulation_step();
  void initialize_events();

  inline Sphere *get_spheres() const { return spheres; }
  inline int get_number_of_spheres() const { return number_of_spheres; }
  inline const std::vector<long double> get_collision_times() const {
    return collision_times;
  }

  inline friend std::ostream &operator<<(std::ostream &out,
                                         const sphere_simulation &s) {
    out << "sphere_simulation { "
        << "number_of_spheres: " << s.number_of_spheres
        << ", epsilon: " << s.epsilon << " }";
    return out;
  }

private:
  // data structs
  std::vector<long double> collision_times; // stores the collision times
  std::priority_queue<Event> event_queue;
  Sphere *spheres;

  // simulation parameters
  long double max_time;
  long double current_time;
  int number_of_spheres;
  double torus_size;
  double epsilon; // radius

  // functions
  void handle_event(Event &event);
  void add_collision_event(Sphere *s1, Sphere *s2);
  void find_collision_events(Sphere *s1);
  void wrap_around(Sphere *s);
  void update_positions(long double dt);
  std::vector<point3> get_images(Sphere *s);
};

#endif