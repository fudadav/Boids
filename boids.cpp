
#include "boid.hpp"
#include "functions.hpp"
#include "save.hpp"
#include "swarm.hpp"
#include "vec3.hpp"
// #include "predator.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

// Funzione per inizializzare i parametri basandosi sull'input dell'utente
void initialize_parameters(bool& manually, int& size, double& wingspan,
                           double& max_speed, double& min_distance,
                           double& separation_factor, double& cohesion_factor,
                           double& allineation_factor, vec3& screen, bool& toro,
                           bool& windbool, vec3& wind, double& attack_speed,
                           double& attack_range)
{
  std::string input;
  std::cout << "Insert parameters manually? (y/N): ";
  std::cin >> input;
  manually = (input == "y" || input == "Y" || input == "yes" || input == "Yes");

  if (manually) {
    std::cout << "Enter swarm size: ";
    std::cin >> size;
    std::cout << "Enter wingspan: ";
    std::cin >> wingspan;
    std::cout << "Enter maximum speed: ";
    std::cin >> max_speed;
    std::cout << "Enter minimum distance: ";
    std::cin >> min_distance;
    std::cout << "Enter separation factor (0 ~ 100): ";
    std::cin >> separation_factor;
    std::cout << "Enter cohesion factor (0 ~ 100): ";
    std::cin >> cohesion_factor;
    std::cout << "Enter allineation factor (0 ~ 100): ";
    std::cin >> allineation_factor;
    std::cout << "Enter width: ";
    std::cin >> screen[0];
    std::cout << "Enter length: ";
    std::cin >> screen[1];
    std::cout << "Enter height: ";
    std::cin >> screen[2];
    std::cout << "Enter predator attack speed: ";
    std::cin >> attack_speed;
    std::cout << "Enter predator attack range: ";
    std::cin >> attack_range;
  }

  std::cout << "Use toroidal space? (y/N): ";
  std::cin >> input;
  toro = (input == "y" || input == "Y" || input == "yes" || input == "Yes");

  std::cout << "Use wind? (y/N): ";
  std::cin >> input;
  windbool = (input == "y" || input == "Y" || input == "yes" || input == "Yes");

  if (windbool) {
    double windspeed;
    std::cout << "Enter wind speed: ";
    std::cin >> windspeed;

    std::srand(static_cast<unsigned>(std::time(0)));
    wind = vec3((std::rand() % 100) - 50, (std::rand() % 100) - 50,
                (std::rand() % 100) - 50)
               .normalize()
         * windspeed;
  }
}

void draw_boids_on_plane(swarm& mySwarm, predator& myPredator,
                         sf::RenderWindow& window, int plane)
{
  sf::CircleShape boidShape(
      mySwarm.get_wingspan()); // Cerchio con raggio 5 per i boids
  boidShape.setFillColor(sf::Color::White);

  sf::CircleShape predatorShape(
      mySwarm.get_wingspan()
      * (8 / 5)); // Cerchio con raggio 8 per il predatore
  predatorShape.setFillColor(sf::Color::Red);

  // Disegno dei boids
  for (int i = 0; i < mySwarm.get_size(); i++) {
    sf::Vector2f position;

    switch (plane) {
    case 0:
      position.x = mySwarm[i].get_position().x;
      position.y = mySwarm[i].get_position().y;
      break;
    case 1:
      position.x = mySwarm[i].get_position().x;
      position.y = mySwarm[i].get_position().z;
      break;
    case 2:
      position.x = mySwarm[i].get_position().y;
      position.y = mySwarm[i].get_position().z;
      break;
    }

    boidShape.setPosition(position);
    window.draw(boidShape);
  }

  // Disegno del predatore
  sf::Vector2f predatorPosition;
  switch (plane) {
  case 0:
    predatorPosition.x = myPredator.get_position().x;
    predatorPosition.y = myPredator.get_position().y;
    break;
  case 1:
    predatorPosition.x = myPredator.get_position().x;
    predatorPosition.y = myPredator.get_position().z;
    break;
  case 2:
    predatorPosition.x = myPredator.get_position().y;
    predatorPosition.y = myPredator.get_position().z;
    break;
  }

  predatorShape.setPosition(predatorPosition);
  window.draw(predatorShape);
}
// FONSI CHE CAZZO FAI NON VA QUI aspetta, è una prova fai vado a pranzo buon
// appetito
boid* find_prey(predator& batman, swarm& swarm)
{
  boid* nearest_prey      = nullptr;
  double nearest_distance = swarm.get_screen().norm();

  for (int i = 0; i < swarm.get_size(); i++) {
    double dist =
        distance(swarm.get_boids()[i].get_position(), batman.get_position());
    if (dist < nearest_distance && dist <= batman.get_attack_range()) {
      nearest_distance = dist;
      nearest_prey     = &swarm.get_boids()[i];
    }
  }
  return nearest_prey;
}

void attack(predator& predator, swarm& swarm)
{
  boid* prey = find_prey(predator, swarm);
  if (prey != nullptr) {
    vec3 direction_to_prey =
        (prey->get_position() - predator.get_position()).normalize();
    predator.set_velocity(direction_to_prey * predator.get_attack_speed());
  }
}

void update_predator(predator& batman, swarm& swarm)
{
  // Aggiorna la posizione e il comportamento del predatore
  attack(batman, swarm);
  batman.update_boid(vec3(), batman.get_attack_speed());
  swarm.rule4(batman);
}

int main()
{
  sf::RenderWindow windowXY(sf::VideoMode(800, 600), "XY Plane");
  sf::RenderWindow windowXZ(sf::VideoMode(800, 600), "XZ Plane");
  sf::RenderWindow windowYZ(sf::VideoMode(800, 600), "YZ Plane");

  int size        = 100;
  double wingspan = 2, max_speed = 1, min_distance = 30;
  double separation_factor = 50, cohesion_factor = 50, allineation_factor = 50;
  vec3 screen(500, 500, 500), wind(0, 0, 0);
  bool toro = false, windbool = false, manually = false;

  double attack_range = 500, attack_speed = 0.9;

  initialize_parameters(manually, size, wingspan, max_speed, min_distance,
                        separation_factor, cohesion_factor, allineation_factor,
                        screen, toro, windbool, wind, attack_range,
                        attack_speed);

  predator batman(vec3(0, 0, 0), vec3(0, 0, 0), attack_range, attack_speed,
                  screen);

  swarm boids(size, wingspan, max_speed, min_distance, separation_factor,
              cohesion_factor, allineation_factor, batman, screen, toro, wind);

  int t = 0;
  while (windowXY.isOpen() && windowXZ.isOpen() && windowYZ.isOpen()) {
    sf::Event event;
    while (windowXY.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        windowXY.close();
    }
    while (windowXZ.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        windowXZ.close();
    }
    while (windowYZ.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        windowYZ.close();
    }

    windowXY.clear();
    draw_boids_on_plane(boids, batman, windowXY, 0);
    windowXY.display();

    windowXZ.clear();
    draw_boids_on_plane(boids, batman, windowXZ, 1);
    windowXZ.display();

    windowYZ.clear();
    draw_boids_on_plane(boids, batman, windowYZ, 2);
    windowYZ.display();

    boids.update_swarm();
    update_predator(batman, boids);

    if (t % 100 == 0) {
      std::vector<double> distances, velocities;
      for (int i = 0; i < size; ++i) {
        velocities.push_back(boids[i].get_velocity().norm());
        for (int j = i + 1; j < size; ++j) {
          distances.push_back(
              toro
                  ? toroidal_distance(boids[i].get_position(),
                                      boids[j].get_position(),
                                      boids.get_screen())
                  : (boids[i].get_position() - boids[j].get_position()).norm());
        }
      }

      double mean_dist = mean(distances), mean_vel = mean(velocities);
      std::cout << "Tempo " << t << ": Distanza Media = " << mean_dist
                << ", Deviazione Distanza = " << dev_std(distances, mean_dist)
                << ", Velocità Media = " << mean_vel
                << ", Deviazione Velocità = " << dev_std(velocities, mean_vel)
                << std::endl;

      save_positions_to_file(boids, "boids_positions.txt");
    }
    ++t;
  }

  return 0;
}
// Fine del codice