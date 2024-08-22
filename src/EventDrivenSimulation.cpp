#include <cmath>
#include <queue>
#include <random>
#include <vector>

#include "EventDrivenSimulation.h"
#include "config.h"

EventDrivenSimulation::EventDrivenSimulation(int n) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::poisson_distribution<int> poisson_dist(n);

  // Generate a random number from the Poisson distribution
  int sphere_count = poisson_dist(gen);

  // epsilon is defined as length^d / n^(1 / d-1)
  double epsilon = pow(TORUS_SIZE, DIMENSIONS) 
                 / pow(n, 1.0 / (double)(DIMENSIONS - 1));
  // number of cells in each dimension
  int num_cells = floor(TORUS_SIZE / epsilon);
  // size of each cell
  long double cell_size = TORUS_SIZE / (long double) num_cells;

  // initialize data structures
  this->current_time = 0.0;
  this->collision_times = std::vector<long double>{};
  this->event_queue = std::priority_queue<Event>{};
  this->grid = SpatialGrid(cell_size, num_cells, sphere_count);
}