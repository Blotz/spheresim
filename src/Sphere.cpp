#include <cmath>

#include "Sphere.h"
#include "vec3.h"

double collide(const Sphere* s1, const Sphere* s2) {
    if (s1->max_collision_checks <= 0 || s2->max_collision_checks <= 0) {
        return -1;
    }

    vec3 relative_velocity = s2->velocity - s1->velocity;
    vec3 relative_position = s2->center - s1->center;

    double combined_radius = s1->radius + s2->radius;

    double a = relative_velocity.dot(relative_velocity);
    double b = 2 * relative_velocity.dot(relative_position);
    double c = relative_position.dot(relative_position) - combined_radius * combined_radius;
    
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        // No real roots, so no collision
        return -1;
    }

    double sqrt_discriminant = sqrt(discriminant);
    double t1 = (-b - sqrt_discriminant) / (2 * a);
    // double t2 = (-b + sqrt_discriminant) / (2 * a);

    if (t1 >= 0 ) {
        return t1;
    }
    return -1;
}

void resolve_collision(Sphere* s1, Sphere* s2) {
    vec3 normal = s2->center - s1->center;
    normal.normalize();
    vec3 relative_velocity = s1->velocity - s2->velocity;

    double velocity_along_normal = relative_velocity.dot(normal);

    // unneeded check
    // assume this is always true
    // // If the spheres are moving apart, no collision occurs
    // if (velocity_along_normal < 0) {
    //     return;
    // }

    vec3 impulse = velocity_along_normal * normal;
    s1->velocity -= impulse;
    s2->velocity += impulse;
}

void Sphere::update_position(long double dt) {
    center += velocity * dt;
}