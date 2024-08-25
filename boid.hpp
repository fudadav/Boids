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

  vec3 get_position() const
  { // restituisce la posizione del boid
    return position;
  }
  vec3 get_velocity() const
  { // restituisce la velocità del boid
    return velocity;
  }
  void set_position(const vec3& new_position)
  { // imposta la posizione del boid
    position = new_position;
  }
  void set_velocity(const vec3& new_velocity)
  { // imposta la velocità del boid
    velocity = new_velocity;
  }
  void update_boid(vec3 delta_v, double max_speed)
  { // aggiorna la posizione del boid
    velocity += delta_v * 0.1;

    // Limita la velocità massima
    if (velocity.norm() > max_speed) {
      velocity = velocity.normalize() * max_speed;
    }

    position += velocity;
  }

  // Copy constructor
  boid(const boid& other)
      : position(other.position)
      , velocity(other.velocity)
  {
    // Copy any other members as needed
  }

  bool operator==(const boid& other)
  { // uguaglianza
    return position.x == other.position.x && position.y == other.position.y
        && position.z == other.position.z && velocity.x == other.velocity.x
        && velocity.y == other.velocity.y && velocity.z == other.velocity.z;
  }
  bool operator!=(const boid& other)
  { // disuguaglianza
    return position.x != other.position.x || position.y != other.position.y
        || position.z != other.position.z || velocity.x != other.velocity.x
        || velocity.y != other.velocity.y || velocity.z != other.velocity.z;
  }

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