#ifndef PREDATOR
#define PREDATOR

#include "boid.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "vec3.hpp"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

class predator : public boid
{
  double attack_range;
  double attack_speed;
  vec3 screen;
  bool toroidal;
  double prefered_height;
  vec3 wind;

  void initialize_predator()
  {
    std::srand(static_cast<unsigned>(std::time(0)));

    double x = std::rand() % static_cast<int>(screen.x);
    double y = std::rand() % static_cast<int>(screen.y);
    double z = std::rand() % static_cast<int>(screen.z);

    set_position(vec3(x, y, z));

    double vx =
        ((std::rand() % static_cast<int>(2 * attack_speed / std::sqrt(3)))
         - (attack_speed / std::sqrt(3)));
    double vy =
        ((std::rand() % static_cast<int>(2 * attack_speed / std::sqrt(3)))
         - (attack_speed / std::sqrt(3)));
    double vz =
        ((std::rand() % static_cast<int>(2 * attack_speed / std::sqrt(3)))
         - (attack_speed / std::sqrt(3)));

    set_velocity(vec3(vx, vy, vz));

    if (get_velocity().norm() > attack_speed) {
      set_velocity(get_velocity().normalize() * attack_speed);
    };
  }

  boid* find_prey(swarm& boids)
  {
    boid* nearest_prey      = nullptr;
    double nearest_distance = screen.norm();

    if (!toroidal) {
      for (int i = 0; i < boids.get_size(); i++) {
        double dist =
            distance(boids.get_boids()[i].get_position(), get_position());
        if (dist < nearest_distance && dist <= get_attack_range()) {
          nearest_distance = dist;
          nearest_prey     = &boids.get_boids()[i];
        }
      }
    } else {
      for (int i = 0; i < boids.get_size(); i++) {
        double dist = toroidal_distance(boids.get_boids()[i].get_position(),
                                        get_position(), screen);
        if (dist < nearest_distance && dist <= get_attack_range()) {
          nearest_distance = dist;
          nearest_prey     = &boids.get_boids()[i];
        }
      }
    }
    return nearest_prey;
  }

  void attack(swarm& boids)
  {
    boid* prey = find_prey(boids);
    if (prey != nullptr) {
      if (!toroidal) {
        vec3 direction_to_prey =
            (prey->get_position() - get_position()).normalize();
        set_velocity(direction_to_prey * get_attack_speed());
      } else {
        vec3 direction_to_prey =
            toroidal_vec_dist(prey->get_position(), get_position(), screen)
                .normalize();
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
      , prefered_height(screen.z / 3)
      , wind(vec3(0, 0, 0))
  {}

  predator(vec3 position = vec3(0, 0, 0), vec3 velocity = vec3(0, 0, 0),
           double attack_range_ = 50, double attack_speed_ = 70,
           vec3 screen_ = vec3(500, 500, 500), bool toroidal_ = 0,
           vec3 wind_ = vec3(0, 0, 0))
      : boid(position, velocity)
      , attack_range(attack_range_)
      , attack_speed(attack_speed_)
      , screen(screen_)
      , toroidal(toroidal_)
      , prefered_height(screen.z / 3)
      , wind(wind_)
  {
    if (attack_range <= 0) {
      throw std::invalid_argument("attack_range must be greater than 0");
    }
    if (attack_speed <= 0) {
      throw std::invalid_argument("attack_speed must be greater than 0");
    }
    initialize_predator();
  }

  void update_predator(swarm& boids)
  {
    if (boids.get_cooldown() >= 1000) {
      attack(boids);
    } else {
      update_boid_velocity(keep_height(*this, prefered_height, 100),
                           attack_speed * 0.6);
    }
    update_boid_velocity(vec3(), attack_speed);
    update_boid(wind,
                wind.norm() + attack_speed); // il vento ha la precedenza sulle
                                             // intenzioni del predator
    boids.border(*this);
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

  // copy constructor
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

#endif