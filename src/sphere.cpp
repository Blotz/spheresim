#include <cmath>

#include "vec3.h"
#include "sphere.h"


double collide(const sphere* s1, const sphere* s2) {
    if (s1->max_collision_checks <= 0 || s2->max_collision_checks <= 0) {
        return -1;
    }

    vec3 relative_velocity = s2->velocity - s1->velocity;
    vec3 relative_position = s2->center - s1->center;

    double combined_radius = s1->radius + s2->radius;

    double a = dot(relative_velocity, relative_velocity);
    double b = 2 * dot(relative_velocity, relative_position);
    double c = dot(relative_position, relative_position) - combined_radius * combined_radius;
    
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

// double sphere::collide(vec3& center, const sphere* other) const {
//     if (this->max_collision_checks <= 0 || other->max_collision_checks <= 0) {
//         return -1;
//     }

//     vec3 relative_velocity = other->velocity - this->velocity;
//     vec3 relative_position = other->center - center;

//     double combined_radius = this->radius + other->radius;

//     double a = dot(relative_velocity, relative_velocity);
//     double b = 2 * dot(relative_velocity, relative_position);
//     double c = dot(relative_position, relative_position) - combined_radius * combined_radius;
    
//     double discriminant = b * b - 4 * a * c;

//     if (discriminant < 0) {
//         // No real roots, so no collision
//         return -1;
//     }

//     double sqrt_discriminant = sqrt(discriminant);
//     double t1 = (-b - sqrt_discriminant) / (2 * a);
//     // double t2 = (-b + sqrt_discriminant) / (2 * a);

//     if (t1 >= 0 ) {
//         return t1;
//     }
//     return -1;
// }

// double sphere::collide(const sphere* other) const {
//     if (this->max_collision_checks <= 0 || other->max_collision_checks <= 0) {
//         return -1;
//     }

//     vec3 relative_velocity = other->velocity - this->velocity;
//     vec3 relative_position = other->center - this->center;

//     double combined_radius = this->radius + other->radius;

//     double a = dot(relative_velocity, relative_velocity);
//     double b = 2 * dot(relative_velocity, relative_position);
//     double c = dot(relative_position, relative_position) - combined_radius * combined_radius;
    
//     double discriminant = b * b - 4 * a * c;

//     if (discriminant < 0) {
//         // No real roots, so no collision
//         return -1;
//     }

//     double sqrt_discriminant = sqrt(discriminant);
//     double t1 = (-b - sqrt_discriminant) / (2 * a);
//     // double t2 = (-b + sqrt_discriminant) / (2 * a);

//     if (t1 >= 0 ) {
//         return t1;
//     }
//     return -1;
// }

void sphere::update_position(double dt) {
    center += velocity * dt;
}

vec3 sphere::collision_velocity(const sphere& other) const {
    vec3 normal = unit_vector(other.center - this->center);
    vec3 relative_velocity = this->velocity - other.velocity;

    double velocity_along_normal = dot(relative_velocity, normal);

    // If the spheres are moving apart, no collision occurs
    if (velocity_along_normal < 0) {
        return this->velocity;
    }

    return this->velocity - velocity_along_normal * normal;
}