#ifndef FUNCTIONS
#define FUNCTIONS

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
#include <string>
#include <vector>

// Funzione per inizializzare i parametri basandosi sull'input dell'utente
void initialize_parameters(bool& manually, int& size, double& wingspan,
                           double& max_speed, double& min_distance,
                           double& separation_factor, double& cohesion_factor,
                           double& alignment_factor, double& fear_factor,
                           double& sight_distance, vec3& screen,
                           bool& toroidalbool, bool& windbool, vec3& wind,
                           double& attack_speed, double& attack_range)
{
  std::string input;
  double windspeed{0.02};

  std::cout << "Enable wind? [y/N]: ";
  std::cin >> input;
  windbool = (input == "y" || input == "Y" || input == "yes" || input == "Yes");

  std::cout << "Enable toroidal space? (Recommended with wind enabled) [y/N]: ";
  std::cin >> input;
  toroidalbool =
      (input == "y" || input == "Y" || input == "yes" || input == "Yes");

  std::cout << "Insert parameters manually? [y/N]: ";
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
    std::cout << "Enter alignment factor (0 ~ 100): ";
    std::cin >> alignment_factor;
    std::cout << "Enter fear factor (0 ~ 100): ";
    std::cin >> fear_factor;
    std::cout << "Enter sight distance: ";
    std::cin >> sight_distance;
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
    if (windbool) {
      std::cout << "Enter wind speed: ";
      std::cin >> windspeed;
    }
  }

  if (windbool) {
    std::srand(static_cast<unsigned>(std::time(0)));
    wind = vec3((std::rand() % 100) - 50, (std::rand() % 100) - 50,
                (std::rand() % 20) - 10)
               .normalize()
         * windspeed;
  }
}

// finestre usate nella funzione draw e nel main
sf::RenderWindow windowXY;
sf::RenderWindow windowXZ;
sf::RenderWindow windowYZ;

// disegna le finestre
void draw_windows()
{
  // Get the desktop resolution
  sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
  unsigned int screenWidth  = desktopMode.width;
  unsigned int screenHeight = desktopMode.height;

  // Define padding and application bar height (adjust as needed)
  int padding      = 12;
  int appBarHeight = 35; // Assume the application bar is 35 pixels high

  // Calculate the size for each window, slightly smaller than the quarter
  // screen
  unsigned int windowWidth  = (screenWidth / 2) - padding;
  unsigned int windowHeight = (screenHeight / 2) - 2 * padding - appBarHeight;

  // Create the windows with the calculated sizes
  windowXY.create(sf::VideoMode(windowWidth, windowHeight), "XY Plane");
  windowXZ.create(sf::VideoMode(windowWidth, windowHeight), "XZ Plane");
  windowYZ.create(sf::VideoMode(windowHeight, windowHeight), "YZ Plane");

  // Set the position of the windows, with offset to account for padding and app
  // bar
  windowXY.setPosition(
      sf::Vector2i(0, windowHeight + 3 * padding)); // Bottom-left corner
  windowXZ.setPosition(sf::Vector2i(0, 0));         // Upper-left corner
  windowYZ.setPosition(
      sf::Vector2i(windowWidth + padding, 0)); // Upper-right corner
}

// disegna i boids nella finestra
void draw_boids_on_plane(swarm& boids, predator& yautja,
                         sf::RenderWindow& window, int plane)
{
  sf::CircleShape boidShape(
      boids.get_wingspan()); // Cerchio con raggio uguale al wingspan
  boidShape.setFillColor(sf::Color::White);

  sf::CircleShape predatorShape(
      boids.get_wingspan() * 2); // Cerchio di dimensione doppia per il predator
  predatorShape.setFillColor(sf::Color::Red);

  vec3 screen = boids.get_screen();
  double width = windowXY.getSize().x;
  double height = windowXY.getSize().y;

  // Disegno dei boids
  for (int i = 0; i < boids.get_size(); i++) {
    sf::Vector2f position;

    switch (plane) {
    case 0:
      position.x = boids[i].get_position().x / screen.x * width;
      position.y = boids[i].get_position().y / screen.y * height;
      break;
    case 1:
      position.x = boids[i].get_position().x / screen.x * width;
      position.y = boids[i].get_position().z / screen.z * height;
      break;
    case 2:
      position.x = boids[i].get_position().y / screen.y * height;
      position.y = boids[i].get_position().z / screen.z * height;
      break;
    }

    boidShape.setPosition(position);
    window.draw(boidShape);
  }

  // Disegno del predator
  sf::Vector2f predatorPosition;
  switch (plane) {
  case 0:
    predatorPosition.x = yautja.get_position().x / screen.x * width;
    predatorPosition.y = yautja.get_position().y / screen.y * height;
    break;
  case 1:
    predatorPosition.x = yautja.get_position().x / screen.x * width;
    predatorPosition.y = yautja.get_position().z / screen.z * height;
    break;
  case 2:
    predatorPosition.x = yautja.get_position().y / screen.y * height;
    predatorPosition.y = yautja.get_position().z / screen.z * height;
    break;
  }

  predatorShape.setPosition(predatorPosition);
  window.draw(predatorShape);
}

// interruzione del programma alla chiusura di una finestra
void handle_events(sf::RenderWindow& window)
{
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }
  }
}

// stampa a schermo delle statistiche
void print_statistics(swarm& boids, int t)
{
  std::vector<double> distances, velocities;
  for (int i = 0; i < boids.get_size(); ++i) {
    velocities.push_back(boids[i].get_velocity().norm());
    for (int j = i + 1; j < boids.get_size(); ++j) {
      distances.push_back(
          boids.is_toroidal()
              ? toroidal_distance(boids[i].get_position(),
                                  boids[j].get_position(), boids.get_screen())
              : (boids[i].get_position() - boids[j].get_position()).norm());
    }
  }
  double mean_dist = mean(distances), mean_vel = mean(velocities);
  std::cout << "t = " << t << "; <d> = " << mean_dist
            << "; sd = " << dev_std(distances, mean_dist)
            << "; <v> = " << mean_vel
            << "; sv  = " << dev_std(velocities, mean_vel)
            << "; n  = " << boids.get_size()
            << std::endl;
}

// update del predator e dei boids e stampa delle statistiche a intervalli
// regolari
void update_simulation(predator& yautja, swarm& boids, int& t)
{
  yautja.update_predator(boids);
  boids.update_swarm(yautja);

  if (t % 100 == 0) {
    print_statistics(boids, t);
  }
  ++t;
}

#endif