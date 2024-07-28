#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>

#include "sphere_simulation.h"
#include "vec3.h"

void draw_sphere(sf::RenderWindow &window, sphere &s, float scale) {
    sf::CircleShape circle(s.get_radius() * scale);
    
    circle.setPosition(s.get_center()[0] * scale, s.get_center()[1] * scale);

    if (s.get_max_collision_checks() == 0) {
      circle.setFillColor(sf::Color::Red);
    } else {
      circle.setFillColor(sf::Color::Green);
    }
    window.draw(circle);
}

void write_data_to_csv(const std::vector<double>& vec, const std::string& filename) {
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
        if (i != vec.size()-1) {
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
    sf::RenderWindow window(sf::VideoMode(800, 800), "Sphere Collision Simulation");

    // Define the scale to fit the simulation into the window
    float scale = 800.0f;  // Assuming your simulation space is in [0, 1]

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
        sphere* spheres = simulation.get_spheres();
        
        for (int i = 0; i < simulation.get_number_of_spheres(); i++) {
            draw_sphere(window, spheres[i], scale);
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

  // run_gui_simulation(10);
  // run_simulation(10);

  // sphere_simulation simulation(100);
  sphere_simulation simulation(10000);
  simulation.initialize_events();
  simulation.run_simulation();
  std::vector<double> collision_times = simulation.get_collision_times();
  std::cout << "Number of collisions: " << collision_times.size() << std::endl;

  return 0;
}

