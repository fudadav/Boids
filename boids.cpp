
#include "boid.hpp"
#include "functions.hpp"
#include "predator.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "vec3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main()
{
  draw_windows();

  int size        = 100;
  double wingspan = 2, max_speed = 1, min_distance = 30, sight_distance = 150;
  double separation_factor = 50, cohesion_factor = 50, alignment_factor = 50,
         fear_factor = 100;
  vec3 screen(600, 300, 300), wind(0, 0, 0);
  bool toroidalbool = false, windbool = false, manually = false, attack = true;

  double attack_range = 300, attack_speed = 1.1;
  if (!attack) {
    attack_range = 0;
    attack_speed = 0;
  }

  initialize_parameters(manually, size, wingspan, max_speed, min_distance,
                        separation_factor, cohesion_factor, alignment_factor,
                        fear_factor, sight_distance, screen, toroidalbool,
                        windbool, wind, attack_speed, attack_range);

  predator yautja(vec3(0, 0, 0), vec3(0, 0, 0), attack_range, attack_speed,
                  screen, toroidalbool, wind);

  swarm boids(size, wingspan, max_speed, min_distance, sight_distance,
              separation_factor, cohesion_factor, alignment_factor, fear_factor,
              yautja, screen, toroidalbool, wind);

  int t = 0;
  while (windowXY.isOpen() && windowXZ.isOpen() && boids.get_size() > 1) {
    handle_events(windowXY);
    handle_events(windowXZ);

    windowXY.clear();
    draw_boids_on_plane(boids, yautja, windowXY, 0);
    windowXY.display();

    windowXZ.clear();
    draw_boids_on_plane(boids, yautja, windowXZ, 1);
    windowXZ.display();

    update_simulation(yautja, boids, t);
  }

  return 0;
}
// Fine del codice
