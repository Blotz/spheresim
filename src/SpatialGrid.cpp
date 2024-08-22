#include <random>
#include <algorithm>
#include <vector>

#include "SpatialGrid.h"
#include "config.h"
#include "vec3.h"
#include "Sphere.h"


SpatialGrid::SpatialGrid(long double cell_size, int grid_size, int num_spheres) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<long double> normal_dist(0, 1);
  std::uniform_real_distribution<long double> uniform_dist(0, 1);

  this->cell_size = cell_size;
  this->grid_size = grid_size;
  this->grid = new GridCell[grid_size * grid_size * grid_size];

  if (this->grid == nullptr) {
    throw std::bad_alloc();
  }

  for (int i = 0; i < num_spheres; i++) {
    point3 center{};
    vec3 velocity{};

    for (int j = 0; j < DIMENSIONS; j++) {
      center[j] = uniform_dist(gen);
      velocity[j] = normal_dist(gen);
    }

    velocity.normalize();

    // radius = epsilon/2
    Sphere *s = new Sphere(cell_size / 2.0, center, velocity);
    add_sphere(s);
  }
}

SpatialGrid::~SpatialGrid() {
  if (this->grid != nullptr) {
    delete[] this->grid;
    this->grid = nullptr;
  }
}

void SpatialGrid::wrap_position(point3 *p) {
  for (int i = 0; i < DIMENSIONS; i++) {
    if ((*p)[i] < MIN_CORNER[i]) {
      (*p)[i] = MAX_CORNER[i] - (MIN_CORNER[i] - (*p)[i]);
    } else if ((*p)[i] > MAX_CORNER[i]) {
      (*p)[i] = MIN_CORNER[i] + ((*p)[i] - MAX_CORNER[i]);
    }
  }
}

int SpatialGrid::get_cell_index(const point3 &p) {
  int index = 0;
  for (int i = 0; i < DIMENSIONS; i++) {
    index += (int) (p[i] / cell_size) * pow(grid_size, i);
  }

  return index;
}

int_point SpatialGrid::get_cell_position(const point3 &p) {
  int_point cell_pos{};
  for (int i = 0; i < DIMENSIONS; i++) {
    cell_pos[i] = (int) (p[i] / cell_size);
  }

  return cell_pos;
}

void SpatialGrid::add_sphere(Sphere *s) {
  point3 &center = s->get_center();
  int cell_index = get_cell_index(center);
  grid[cell_index].spheres.push_back(s);
}

void SpatialGrid::update_sphere(const point3 &old_pos, Sphere *s) {
  int old_cell_index = get_cell_index(old_pos);
  int new_cell_index = get_cell_index(s->get_center());

  if (old_cell_index != new_cell_index) {
    // remove sphere from old cell. This is a linear search
    auto &old_cell = grid[old_cell_index].spheres;
    auto it = std::find(old_cell.begin(), old_cell.end(), s);
    if (it != old_cell.end()) {
        old_cell.erase(it);
    }

    // add sphere to new cell
    grid[new_cell_index].spheres.push_back(s);
  }
}

std::vector<Sphere *> SpatialGrid::get_nearby_spheres(Sphere *s) {
  std::vector<Sphere *> nearby_spheres;
  point3 &center = s->get_center();
  int cell_index = get_cell_index(center);

  // get spheres from the same cell
  nearby_spheres.insert(nearby_spheres.end(), grid[cell_index].spheres.begin(),
                        grid[cell_index].spheres.end());

  // get spheres from neighboring cells
  std::vector<GridCell *> nearby_cells = get_nearby_cells(cell_index);
  for (auto &cell : nearby_cells) {
    nearby_spheres.insert(nearby_spheres.end(), cell->spheres.begin(),
                          cell->spheres.end());
  }

  return nearby_spheres;
}

std::vector<GridCell *> SpatialGrid::get_nearby_cells(int cell_index) {
  std::vector<GridCell *> nearby_cells;
  point3 cell_pos{};
  for (int i = 0; i < DIMENSIONS; i++) {
    cell_pos[i] = (cell_index / (int) pow(grid_size, i)) % grid_size;
  }

  for (int i = 0; i < pow(3, DIMENSIONS); i++) {
    point3 neighbor_pos = cell_pos;
    for (int j = 0; j < DIMENSIONS; j++) {
      int shift = (i / (int) pow(3, j)) % 3 - 1;
      neighbor_pos[j] += shift;
    }

    wrap_position(&neighbor_pos);
    int neighbor_index = get_cell_index(neighbor_pos);
    nearby_cells.push_back(&grid[neighbor_index]);
  }

  return nearby_cells;
}
