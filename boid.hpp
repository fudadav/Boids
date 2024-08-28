#ifndef BOID
#define BOID

#include "vec3.hpp"

class boid
{
  vec3 position;
  vec3 velocity;

 public:
  boid()
      : position(vec3(0, 0, 0))
      , velocity(vec3(0, 0, 0))
  {}
  boid(vec3 position_, vec3 velocity_)
      : position(position_)
      , velocity(velocity_)
  {}

  // restituisce la posizione del boid
  vec3 get_position() const
  {
    return position;
  }

  // restituisce la velocità del boid
  vec3 get_velocity() const
  {
    return velocity;
  }

  // imposta la posizione del boid
  void set_position(const vec3& new_position)
  {
    position = new_position;
  }

  // imposta la velocità del boid
  void set_velocity(const vec3& new_velocity)
  {
    velocity = new_velocity;
  }

  void update_boid_velocity(vec3 delta_v, double max_speed)
  {
    velocity += delta_v;

    // Limita la velocità massima
    if (velocity.norm() > max_speed) {
      velocity = velocity.normalize() * max_speed;
    }
  }

  // aggiorna la posizione del boid
  void update_boid(vec3 delta_v, double max_speed)
  {
    velocity += delta_v;

    // Limita la velocità massima
    if (velocity.norm() > max_speed) {
      velocity = velocity.normalize() * max_speed;
    }

    position += velocity * 3;
  }

  // Copy constructor
  boid(const boid& other)
      : position(other.position)
      , velocity(other.velocity)
  {}

  // uguaglianza
  bool operator==(const boid& other) const
  {
    return position == other.position && velocity == other.velocity;
  }

  // disuguaglianza
  bool operator!=(const boid& other) const
  {
    return !(*this == other);
  }

  // assegna
  boid operator=(const boid& other)
  {
    if (this != &other) {
      position = other.position;
      velocity = other.velocity;
    }
    return *this;
  }
};

#endif