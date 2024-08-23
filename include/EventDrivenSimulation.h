#ifndef EVENT_DRIVEN_SIMULATION_H
#define EVENT_DRIVEN_SIMULATION_H

#include <queue>
#include <vector>

#include "SpatialGrid.h"
#include "Sphere.h"
#include "config.h"
#include "vec3.h"

struct Event {
  long double time;
  Sphere *s1;
  Sphere *s2;

  Event(long double time, Sphere *s1, Sphere *s2)
      : time(time), s1(s1), s2(s2) {}

  inline friend bool operator<(const Event &e1, const Event &e2) {
    return e1.time > e2.time;
  }
};

class EventDrivenSimulation {
public:
  EventDrivenSimulation(int n);
  // ~EventDrivenSimulation(); default destructor is fine

  void initialize_events();
  void run_simulation();
  void run_simulation_step();

  std::vector<long double> get_collision_times() { return collision_times; }

private:
  long double current_time;

  std::vector<long double> collision_times;
  std::priority_queue<Event> event_queue;
  SpatialGrid grid;


  void handle_event(Event &event);
  void find_collision_events(Sphere *s);
  void update_positions(long double dt);
  void add_collision(Sphere *s1, Sphere *s2);
};

#endif // EVENT_DRIVEN_SIMULATION_H