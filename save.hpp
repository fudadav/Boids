#ifndef SAVE
#define SAVE

#include "vec3.hpp"
#include "swarm.hpp"

#include <fstream>

void save_positions_to_file(swarm boids, const std::string& filename)
{
  // std::ofstream file(filename);
  std::ofstream file(filename, std::ios::app);

  if (file.is_open()) {
    for (int i = 0; i < boids.get_size(); i++) {
      vec3 pos = boids[i].get_position();
      file << pos << ",";
    }
    file << std::endl;
    file.close();
  } else {
    std::cerr << "Errore: impossibile salvare le posizioni sul file " << filename << std::endl;
  }
}

#endif