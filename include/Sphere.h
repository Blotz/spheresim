#ifndef SPHERE_H
#define SPHERE_H

#include <ostream>

#include "config.h"
#include "vec3.h"

struct UUID {
  int id;
  UUID() {static int next_id = 0; id = next_id++;}
  UUID(int id) : id(id) {}
  inline friend bool operator==(const UUID &u1, const UUID &u2) {
    return u1.id == u2.id;
  }
  inline friend bool operator!=(const UUID &u1, const UUID &u2) {
    return !(u1 == u2);
  }
};

class Sphere {
public:
  Sphere() = default;
  Sphere(double radius, point3 center, vec3 velocity)
      : radius(fmax(0, radius)), center(center), velocity(velocity) {
    this->max_collision_checks = MAX_COLLISIONS_CHECKS;
    this->id = UUID();
  }

  /**
   * Checks to see whether two spheres collide. This is done by checking the
   * discriminant of the quadratic equation that is formed by the relative
   * velocity and relative position of the two spheres. If the discriminant is
   * greater than 0, then the spheres collide.
   *
   * returns -1 if there is no collision, otherwise returns the time of
   * collision.
   */
  friend double collide(const Sphere *s1, const Sphere *s2);

  /**
   * resolves the collision between two spheres. 
  */
  friend void resolve_collision(Sphere *s1, Sphere *s2);


  /**
   * calculates the velocity of the sphere after a collision with another
   * sphere. all spheres are assumed to have the same mass and collisions are
   * elastic.
   */
  // vec3 collision_velocity(const Sphere *other);

  /**
   * updates the position of the sphere by time dt.
   */
  void update_position(long double dt);

  /**
   * decrements the number of collision checks left for the sphere.
   */
  inline void decrement_collision_checks() { this->max_collision_checks--; }

  // vec3 normalize_position(const vec3& pos) const;
  // vec3 toroidal_distance(const vec3& pos1, const vec3& pos2) const;
  // void wrap_around();

  void set_velocity(vec3 v) { this->velocity = v; }
  void set_position(point3 p) { this->center = p; }

  vec3 &get_velocity() { return this->velocity; }
  point3 &get_center() { return this->center; }
  // point3 *get_center() { return &this->center; }
  int get_max_collision_checks() { return this->max_collision_checks; }
  double get_radius() { return this->radius; }

  inline friend std::ostream &operator<<(std::ostream &out, const Sphere &s) {
    out << "sphere { radius: " << s.radius << ", center: " << s.center
        << ", velocity: " << s.velocity << ", " << s.max_collision_checks
        << " }";
    return out;
  }
  inline friend bool operator==(const Sphere &s1, const Sphere &s2) {
    return s1.id == s2.id;
  }
  inline friend bool operator!=(const Sphere &s1, const Sphere &s2) {
    return s1.id != s2.id;
  }

private:
  double radius;
  point3 center;
  vec3 velocity;
  int max_collision_checks;
  UUID id;
};

#endif