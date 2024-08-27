#ifndef PREDATOR_H
#define PREDATOR_H

#include "boid.hpp"
#include "swarm.hpp"
#include "vec3.hpp"
#include <cstdlib>
#include <ctime>
#include <stdexcept>

class predator : public boid
{
  double attack_range;
  double attack_speed;
  vec3 screen;
  bool toroidal;

  void bounce() // rimbalzo
  {
    vec3 bounce = get_velocity();
    for (int i = 0; i < 3; i++) {
      if (get_position()[i] < 0 && get_velocity()[i] < 0) {
        bounce[i] *= -1;
      } else if (get_position()[i] > screen[i] && get_velocity()[i] > 0) {
        bounce[i] *= -1;
      }
    }
    set_velocity(bounce);
  }

  void teleport() // teletrasporto
  {
    vec3 new_position = get_position();

    for (int i = 0; i < 3; ++i) {
      if (new_position[i] > screen[i]) {
        new_position[i] = new_position[i] - screen[i];
      } else if (new_position[i] < 0) {
        new_position[i] = screen[i] - new_position[i];
      }
    }

    set_position(new_position);
  }

  void rule4() // rimbalzo
  {
    if (toroidal) {
      teleport();
    } else {
      bounce();
    }
  }

  boid* find_prey(swarm& swarm)
  {
    boid* nearest_prey      = nullptr;
    double nearest_distance = screen.norm();

    if (!toroidal) {
      for (int i = 0; i < swarm.get_size(); i++) {
        double dist =
            distance(swarm.get_boids()[i].get_position(), get_position());
        if (dist < nearest_distance && dist <= get_attack_range()) {
          nearest_distance = dist;
          nearest_prey     = &swarm.get_boids()[i];
        }
      }
    } else {
      for (int i = 0; i < swarm.get_size(); i++) {
        double dist = toroidal_distance(swarm.get_boids()[i].get_position(),
                                        get_position(), screen);
        if (dist < nearest_distance && dist <= get_attack_range()) {
          nearest_distance = dist;
          nearest_prey     = &swarm.get_boids()[i];
        }
      }
    }
    return nearest_prey;
  }

  void attack(swarm& swarm)
  {
    boid* prey = find_prey(swarm);
    if (prey != nullptr) {
      if (!toroidal) {
        vec3 direction_to_prey =
            (prey->get_position() - get_position()).normalize();
        set_velocity(direction_to_prey * get_attack_speed());
      } else {
        vec3 direction_to_prey =
            toroidal_vec_dist(prey->get_position(), get_position(), screen).normalize();
        set_velocity(direction_to_prey * get_attack_speed());
      }
    }
  }

 public:
  predator()
      : boid()
      , attack_range(0)
      , attack_speed(0)
      , screen(vec3(500, 500, 500))
      , toroidal(0)
  {}

  predator(vec3 position = vec3(0, 0, 0), vec3 velocity = vec3(0, 0, 0),
           double attack_range_ = 50, double attack_speed_ = 70,
           vec3 screen_ = vec3(500, 500, 500), bool toroidal_ = 0)
      : boid(position, velocity)
      , attack_range(attack_range_)
      , attack_speed(attack_speed_)
      , screen(screen_)
      , toroidal(toroidal_)
  {
    if (attack_range <= 0) {
      throw std::invalid_argument("attack_range must be greater than 0");
    }
    if (attack_speed <= 0) {
      throw std::invalid_argument("attack_speed must be greater than 0");
    }
    initialize_predator();
  }

  void update_predator(swarm& swarm)
  {
    // Aggiorna la posizione e il comportamento del predatore
    attack(swarm);
    update_boid(vec3(), get_attack_speed());
    rule4();
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
