#ifndef PREDATOR_H
#define PREDATOR_H

#include "boid.hpp"
#include "vec3.hpp"
#include <cstdlib>
#include <ctime>
#include <stdexcept>

class predator : public boid
{
  double attack_range;
  double attack_speed;
  vec3 screen;

 public:
  predator()
      : boid()
      , attack_range(0)
      , attack_speed(0)
      , screen(vec3(500, 500, 500))
  {}

  predator(vec3 position = vec3(0, 0, 0), vec3 velocity = vec3(0, 0, 0),
           double attack_range_ = 50, double attack_speed_ = 70,
           vec3 screen_ = vec3(500, 500, 500))
      : boid(position, velocity)
      , attack_range(attack_range_)
      , attack_speed(attack_speed_)
      , screen(screen_)
  {
    if (attack_range <= 0) {
      throw std::invalid_argument("attack_range must be greater than 0");
    }
    if (attack_speed <= 0) {
      throw std::invalid_argument("attack_speed must be greater than 0");
    }
    initialize_predator();
  }

  double get_attack_range()
  {
    return attack_range;
  }
  double get_attack_speed()
  {
    return attack_speed;
  }
  vec3 get_screen()
  {
    return screen;
  }

  void initialize_predator()
  {
    std::srand(static_cast<unsigned>(std::time(0)));

    double x = std::rand() % static_cast<int>(screen.x);
    double y = std::rand() % static_cast<int>(screen.y);
    double z = std::rand() % static_cast<int>(screen.z);

    set_position(vec3(x, y, z));

    double vx = ((std::rand() % 100) - 50);
    double vy = ((std::rand() % 100) - 50);
    double vz = ((std::rand() % 100) - 50);

    if (vx == 0 && vy == 0 && vz == 0) {
      vx = 10;
    }
    set_velocity(vec3(vx, vy, vz));
  }

  // User-defined copy constructor
  predator(const predator& other)
      : boid(other)
      , attack_range(other.attack_range)
      , attack_speed(other.attack_speed)
      , screen(other.screen)
  {}

  predator& operator=(const predator& other)
  {
    if (this != &other) {
      set_position(other.get_position());
      set_velocity(other.get_velocity());
      attack_range = other.attack_range;
      attack_speed = other.attack_speed;
      screen       = other.screen;
    }
    return *this;
  }
};

#endif // PREDATOR_H
