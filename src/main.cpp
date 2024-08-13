#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>

#include "sphere_simulation.h"
#include "vec3.h"

void draw_sphere(sf::RenderWindow &window, sphere &s, float scale,
                 float center) {
  sf::CircleShape shape(s.get_radius() * scale);
  if (s.get_max_collision_checks() > 0) {
    shape.setFillColor(sf::Color::Green);
  } else {
    shape.setFillColor(sf::Color::Red);
  }
  shape.setOrigin(shape.getRadius(), shape.getRadius());
  shape.setPosition((s.get_center()[0] + center) * scale,
                    (s.get_center()[1] + center) * scale);
  window.draw(shape);
}

void write_data_to_csv(const std::vector<double> &vec,
                       const std::string &filename) {
  // Create an output file stream
  std::ofstream file(filename, std::ios::app);

  // Check if the file is open
  if (!file.is_open()) {
    std::cerr << "Unable to open file: " << filename << std::endl;
    return;
  }

  // Iterate through the vector and write each element to the file
  for (size_t i = 0; i < vec.size(); ++i) {
    file << vec[i];
    // Add a comma after each element except the last one
    if (i != vec.size() - 1) {
      file << ",";
    }
  }
  // Add a new line at the end of the file
  file << std::endl;
  // Close the file stream
  file.close();
  std::cout << "Vector successfully written to " << filename << std::endl;
}

void run_gui_simulation(int n) {
  // Create the simulation
  sphere_simulation simulation(n);
  simulation.initialize_events();

  // Create the window
  sf::RenderWindow window(sf::VideoMode(800, 800),
                          "Sphere Collision Simulation");

  // Define the scale to fit the simulation into the window
  float scale = 800.0;
  float center = 0.0;

  // Main loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // Update simulation
    simulation.run_simulation_step();

    // Clear the window
    window.clear();

    // Draw the spheres
    sphere *spheres = simulation.get_spheres();

    for (int i = 0; i < simulation.get_number_of_spheres(); i++) {
      draw_sphere(window, spheres[i], scale, center);
    }

    // Display the window
    window.display();
  }
}

void run_simulation(int n) {
  sphere_simulation simulation(n);
  std::cout << simulation << std::endl;

  simulation.initialize_events();
  simulation.run_simulation();

  std::vector<double> collision_times = simulation.get_collision_times();

  std::cout << "Number of collisions: " << collision_times.size() << std::endl;

  write_data_to_csv(collision_times, "collision_times.csv");
}

int main() {
  // for (int i = 0; i < 5; i++) {
  //   std::cout << "Running simulation " << i << std::endl;
  //   run_simulation(10000);
  // }

  //   run_gui_simulation(10000);
  // run_simulation(10);

  //   return 0;

  // sphere_simulation simulation(10000);
  // simulation.initialize_events();
  // simulation.run_simulation();
  // std::cout << simulation << std::endl;
  // std::vector<double> collision_times = simulation.get_collision_times();
  // std::cout << "Number of collisions: " << collision_times.size() <<
  // std::endl;

  // return 0;

  point3 p1 = point3(0.2, 0.0, 0.0);
  point3 p2 = point3(0.9, 0.0, 0.0);
  vec3 v1 = vec3(-1.0, 0.0, 0.0);
  vec3 v2 = vec3(0.0, 0.0, 0.0);

  sphere s1 = sphere(0.1, p1, v1, 10, 1.0);
  sphere s2 = sphere(0.1, p2, v2, 10, 1.0);
  
  std::cout << s1 << std::endl;
  std::cout << s2 << std::endl;
  
  std::cout << s1 << " collides " << s2 << " in " << collide(&s1, &s2) << std::endl;
  double t = collide(&s1, &s2);

  s1.update_position(t);
  s2.update_position(t);

  v1 = s1.collision_velocity(s2);
  v2 = s2.collision_velocity(s1);

  s1.set_velocity(v1);
  s2.set_velocity(v2);

  std::cout << s1.get_center() << std::endl;
  std::cout << s2.get_center() << std::endl;

  std::cout << s1 << std::endl;
  std::cout << s2 << std::endl;
  return 0;
}
