#include "vec3.h"
#include "sphere.h"

double sphere::collide(const sphere* other) const {
    if (this->max_collision_checks <= 0 || other->max_collision_checks <= 0) {
        return -1;
    }

    vec3 relative_velocity = this->velocity - other->velocity;
    vec3 relative_position = this->center - other->center;

    double a = relative_velocity.length_squared();
    double b = 2 * dot(relative_velocity, relative_position);
    double c = relative_position.length_squared() - (this->radius + this->radius) * (other->radius + other->radius);
    
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        // No real roots, so no collision
        return -1;
    }

    double sqrt_discriminant = sqrt(discriminant);
    double t1 = (-b - sqrt_discriminant) / (2 * a);
    double t2 = (-b + sqrt_discriminant) / (2 * a);
    // currently not using t2, but it could be useful for debugging
    // t2 is the time when the spheres exit collision

    // Find the smallest positive time within the interval [0, dt]
    double collision_time = -1;
    if (t1 >= 0) {
        collision_time = t1;
    }
    if (t2 >= 0) {
        if (collision_time != -1) {
            collision_time = std::min(collision_time, t2);
        } else {
            collision_time = t2;
        }
    }

    return collision_time;
}

vec3 sphere::collision_velocity(const sphere& other) const {
    vec3 normal = unit_vector(other.center - this->center);
    vec3 relative_velocity = this->velocity - other.velocity;

    double velocity_along_normal = dot(relative_velocity, normal);

    // If the spheres are moving apart, no collision occurs
    if (velocity_along_normal > 0) {
        return this->velocity;
    }

    return this->velocity - velocity_along_normal * normal;
}