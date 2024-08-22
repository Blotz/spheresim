#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>

#include "sphere_simulation.h"
#include "vec3.h"

/**
 * Writes the data in the vector to a csv file.
 * Each new line contains a single run of the sim
 */
void write_data_to_csv(const std::vector<long double> &vec,
                       const std::string &filename) {
  // Create an output file stream
  std::ofstream file(filename, std::ios::app);

  // Check if the file is open
  if (!file.is_open()) {
    std::cerr << "Unable to open file: " << filename << std::endl;
    return;
  }

  // Iterate through the vector and write each element to the file
  for (int i = 0; i < vec.size(); ++i) {
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

/**
 * This is the code that should be run to generate the data for the report
 * I expect to get roughly 8000 collisions for 10000 spheres.
 * currently it only gets 3000~ collisions.
 */
void run_simulation(int n) {
  sphere_simulation simulation(n);
  std::cout << simulation << std::endl;

  simulation.initialize_events();
  simulation.run_simulation();

  std::vector<long double> collision_times = simulation.get_collision_times();

  std::cout << "Number of collisions: " << collision_times.size() << std::endl;

  write_data_to_csv(collision_times, "collision_times.csv");
}


/**
 * Debugging code to test if the simulation is working correctly.
*/
int main() {
  // run_simulation(10000);
  std::cout << "Starting simulation" << std::endl;

  sphere_simulation simulation(1000);
  std::cout << simulation << std::endl;

  simulation.initialize_events();
  std::cout << "Running simulation" << std::endl;

  simulation.run_simulation();
  std::vector<long double> collision_times = simulation.get_collision_times();
  std::cout << "Number of collisions: " << collision_times.size() <<
  std::endl;

  return 0;
}
