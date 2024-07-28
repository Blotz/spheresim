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
  ~sphere_simulation();

  /**
   * runs the simulation until the max_time is reached or there are no more
   * events.
   */
  void run_simulation();

  /**
   * runs the simulation for one time step until there is no more events.
   */
  void run_simulation_step();

  /**
   * loops though all possible collision events and adds them to the event
   * queue. Must be run before the simulation starts.
   */
  void initialize_events();

  /**
   * nice print for the sphere_simulation class.
   */
  inline friend std::ostream &operator<<(std::ostream &out,
                                         const sphere_simulation &s) {
    out << "sphere_simulation { number_of_spheres: " << s.number_of_spheres
        << ", dimensions: " << s.dimensions << ", epsilon: " << s.epsilon
        << " }";
    return out;
  }

  sphere* get_spheres() const {
    return spheres;
  }
  const int get_number_of_spheres() const {
    return number_of_spheres;
  }
  const std::vector<double> get_collision_times() const {
    return collision_times;
  }

private:
  std::priority_queue<Event> event_queue;
  // std::vector<sphere *> spheres;
  // store spheres in static array to avoid memory fragmentation
  sphere *spheres;

  double max_time;
  double current_time;


  /**
   * handles what happens when a collision event occurs.
   */
  void handle_event(const Event &event);
  /**
   * updates the positions of all spheres by time.
   */
  void update_positions(double dt);
  /**
   * adds a collision event to the event queue.
   */
  void add_collision_event(sphere *s1, sphere *s2);
  /**
   * checks if the sphere goes out of bounds and checks its periodic position.
   */
  void add_possible_collision_event(sphere *s1, sphere *s2);
  void add_possible_collision_event(sphere*);
  /**
   * translates the sphere into the bounds of the simulation.
   */
  void translate_into_bounds(sphere *s);

  int number_of_spheres;
  int max_collision_checks;
  int dimensions;
  double epsilon; // radius

  std::vector<double> collision_times; // stores the collision times for each sphere
};

#endif