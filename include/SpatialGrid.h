#ifndef SPATIAL_GRID_H
#define SPATIAL_GRID_H

#include <vector>

#include "config.h"
#include "Sphere.h"
#include "vec3.h"

#define TORUS_SIZE 1.0
#define MIN_CORNER point3::Zero()
#define MAX_CORNER point3::Ones()

struct GridCell {
  std::vector<Sphere *> spheres;
};

class SpatialGrid {
public:
  SpatialGrid() = default;
  SpatialGrid(long double cell_size, int grid_size, int sphere_count, double sphere_radius);
  SpatialGrid(long double cell_size, int grid_size, int sphere_count, Sphere *spheres);
  ~SpatialGrid();

  std::vector<Sphere *> get_nearby_spheres(Sphere *s);
  void update_positions(long double dt);
  // long double collide(Sphere *s1, Sphere *s2);

  Sphere* get_spheres() const { return spheres; }

private:
  GridCell* grid;
  Sphere* spheres;
  long double cell_size;
  int grid_size;
  int sphere_count;

  // if the position is outside the grid, wrap it around
  // this can be used when getting neighboring cells
  // or when adding a sphere to the grid
  void wrap_position(point3 *p); 
  int get_cell_index(const point3 &p);
  
  void add_sphere(Sphere *s);
  void update_sphere(const point3 &old_pos, Sphere *s);

  std::vector<GridCell *> get_nearby_cells(int cell_index);
};


#endif // SPATIAL_GRID_H