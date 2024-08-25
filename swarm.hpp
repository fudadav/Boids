#ifndef SWARM
#define SWARM

#include "boid.hpp"
#include "functions.hpp"
#include "predator.hpp"
#include "vec3.hpp"

#include <cstdlib>
#include <ctime> // Necessario per std::time
#include <random>
#include <vector>

class swarm
{
  std::vector<boid> boids;

  // vec3 mass_center;
  int size;
  int wingspan;
  double max_speed;
  double min_distance;
  double separation_factor;  // si allontana della distanza tra i boid *
                             // separation_factor
  double cohesion_factor;    // si avvicina di quella percentuale da 1 a 100 al
                             // centro di massa
  double allineation_factor; // si avvicina alla velocità generale di un
                             // ottavo della differenza tra la velocità
                             // percepita e la propria * allineation_factor
  predator batman;
  vec3 screen;
  bool toroidal;
  vec3 wind;

  void initialize_positions()
  {
    std::srand(static_cast<unsigned>(
        std::time(0))); // Inizializza il seme per i numeri casuali

    for (auto& b : boids) {
      double x = std::rand() % static_cast<int>(screen.x);
      double y = std::rand() % static_cast<int>(screen.y);
      double z = std::rand() % static_cast<int>(screen.z);

      // Imposta la posizione del boid
      b.set_position(vec3(x, y, z));

      // LA VELOCITà INIZIALE NON HA SENSO
      //  genera velocità casuali
      double vx = (((std::rand() % 100) - 50) / 50) * max_speed;
      double vy = (((std::rand() % 100) - 50) / 50) * max_speed;
      double vz = (((std::rand() % 100) - 50) / 50) * max_speed;

      b.set_velocity(vec3(vx, vy, vz)
                     / 10); // Imposta la velocità iniziale del boid
    }
  }

  /*vec3 calculate_center_of_mass() // Calcola il centro di massa dello stormo
                                  // considerando tutti i boid in uno spazio
                                  // toroidale
  {
    vec3 center_of_mass = boids[0].get_position();
    for (int i = 0; i < 3; ++i) {
      for (int j = 1; j < size; ++j) {
        if (std::abs(boids[j].get_position()[i] - center_of_mass[i])
            < screen[i] / 2) {
          center_of_mass[i] =
              (j * center_of_mass[i] + boids[j].get_position()[i]) / (j + 1);
        } else {
          center_of_mass[i] =
              (j * center_of_mass[i] + boids[j].get_position()[i] + screen[i])
              / (j + 1);
          if (center_of_mass[i] > screen[i]) {
            center_of_mass[i] -= screen[i];
          }
        }
      }
    }
    return center_of_mass;
  }*/ //non serve più

  // REGOLE DI VOLO

  vec3 rule1(boid& b) // tutti i boid tendono al centro di massa dello stormo,
  {
    vec3 perceived_center(0, 0, 0);
    if (!toroidal) { // bordo
      for (int i = 0; i < size; i++) {
        if (boids[i] != b) {
          perceived_center += boids[i].get_position();
        }
      }
      perceived_center /= size - 1;
      return ((perceived_center - b.get_position()) / 100000) * cohesion_factor;
    } else { // toroidale
      for (int i = 0; i < 3; ++i) {
        int k{};
        for (int j = 0; j < size; ++j) {
          if (boids[j] != b) {
            if (std::abs(boids[j].get_position()[i] - perceived_center[i])
                < screen[i] / 2) {
              perceived_center[i] =
                  (k * perceived_center[i] + boids[j].get_position()[i])
                  / (k + 1);
            } else {
              perceived_center[i] = (k * perceived_center[i]
                                     + boids[j].get_position()[i] + screen[i])
                                  / (k + 1);
              if (perceived_center[i] > screen[i]) {
                perceived_center[i] -= screen[i];
              }
            }
            ++k;
          }
        }
      }
      return (toroidal_vec_dist(perceived_center, b.get_position(), screen)
              / 100000)
           * cohesion_factor;
      // return (toroidal_vec_dist(mass_center, b.get_position(), screen) /
      // 100000) * cohesion_factor;

    } // NON SO COSA NON FUNZIONI MA NON FUNZIONA DIO MADONNA
  }

  vec3 rule2(boid& b) // tutti i boid si tengono a distanza minima tra loro,
  {
    vec3 c(0, 0, 0);
    if (!toroidal) { // bordo
      for (int i = 0; i < size; i++) {
        if (boids[i] != b) {
          if (distance(boids[i].get_position(), b.get_position())
              <= min_distance) {
            c -= (boids[i].get_position() - b.get_position()).normalize()
               / ((boids[i].get_position() - b.get_position()).norm() + 0.0001);
          }
        }
      }
    } else { // toroidale
      for (int i = 0; i < size; i++) {
        if (boids[i] != b) {
          if (toroidal_distance(boids[i].get_position(), b.get_position(),
                                screen)
              <= min_distance) {
            c -= toroidal_vec_dist(boids[i].get_position(), b.get_position(),
                                   screen)
                     .normalize()
               / (toroidal_vec_dist(boids[i].get_position(), b.get_position(),
                                    screen)
                      .norm()
                  + 0.0001);
          }
        }
      }
    }
    return c * separation_factor / 100;
  }

  vec3 rule3(boid& b) // tutti i boid tendono a muoversi nella stessa direzione
  {
    vec3 pv(0, 0, 0);
    for (int i = 0; i < size; i++) {
      if (boids[i] != b) {
        pv += boids[i].get_velocity();
      }
    }
    pv /= size - 1;
    return (pv - b.get_velocity()) / 1000 * allineation_factor;
  }

  void bounce(boid& b) // rimbalzo
  {
    vec3 bounce = b.get_velocity();
    for (int i = 0; i < 3; i++) {
      if (b.get_position()[i] < 0 && b.get_velocity()[i] < 0) {
        bounce[i] *= -1;
      } else if (b.get_position()[i] > screen[i] && b.get_velocity()[i] > 0) {
        bounce[i] *= -1;
      }
    }
    b.set_velocity(bounce);
  }

  void teleport(boid& b) // teletrasporto
  {
    vec3 new_position = b.get_position();

    for (int i = 0; i < 3; ++i) {
      if (new_position[i] > screen[i]) {
        new_position[i] = new_position[i] - screen[i];
      } else if (new_position[i] < 0) {
        new_position[i] = screen[i] - new_position[i];
      }
    }

    b.set_position(new_position);
  }

  vec3 avoid_predator(boid& b) // evita il predatore
  {
    vec3 evade_vector(0, 0, 0);
    vec3 distance_to_predator = b.get_position() - batman.get_position();
    if (distance_to_predator.norm() == 0) {
      evade_vector = b.get_velocity() * max_speed;
    } else if (distance_to_predator.norm() <= batman.get_attack_range()) {
      evade_vector = distance_to_predator.normalize() * separation_factor;
    }
    return evade_vector * separation_factor / (distance_to_predator.norm() + 0.0001);
  }

  boid* find_prey()
  {
    boid* nearest_prey      = nullptr;
    double nearest_distance = screen.norm();

    for (int i = 0; i < size; i++) {
      double dist = distance(boids[i].get_position(), batman.get_position());
      if (dist < nearest_distance && dist <= batman.get_attack_range()) {
        nearest_distance = dist;
        nearest_prey     = &boids[i];
      }
    }
    return nearest_prey;
  }

  // Metodo specifico per l'attacco
  void attack(predator& predator)
  {
    boid* prey = find_prey();
    if (prey != nullptr) {
      vec3 direction_to_prey =
          (prey->get_position() - predator.get_position()).normalize();
      predator.set_velocity(direction_to_prey * predator.get_attack_speed());
    }
  }

 public:
  swarm(int size_, int wingspan_,
        double max_speed_    = 50, // la maxspeed compare 2 volte
        double min_distance_ = 10, double separation_factor_ = 1.5,
        double cohesion_factor_ = 0.5, double allineation_factor_ = 1,
        predator batman = predator(vec3(0, 0, 0), vec3(0, 0, 0), 50, 70,
                                   vec3(500, 500, 500)),
        vec3 screen_ = vec3(500, 500, 500), bool toroidal_ = 0,
        vec3 wind_ = vec3(0, 0, 0))
      : size(size_)
      , wingspan(wingspan_)
      , max_speed(max_speed_)
      , min_distance(min_distance_)
      , separation_factor(separation_factor_)
      , cohesion_factor(cohesion_factor_)
      , allineation_factor(allineation_factor_)
      , batman(batman)
      , screen(screen_) // Imposta larghezza e altezza dello schermo
      , toroidal(toroidal_)
      , wind(wind_)
  {
    if (size < 1) {
      throw std::invalid_argument("swarm size must be greater than 0");
    }
    if (wingspan <= 0) {
      throw std::invalid_argument("wingspan must be greater than 0");
    }
    if (max_speed <= 0) {
      throw std::invalid_argument("max_speed must be greater than 0");
    }
    if (min_distance <= 0) {
      throw std::invalid_argument("min_distance must be greater than 0");
    }
    if (allineation_factor <= 0 || allineation_factor >= 100) {
      throw std::invalid_argument(
          "allineation_factor must be greater than 0 and smaller than 100");
    }
    if (cohesion_factor <= 0
        || cohesion_factor > 100) { // è l'unico per cui 100 va bene!!!!!!!!1
      throw std::invalid_argument(
          "cohesion_factor must be greater than 0 and smaller then 100");
    }
    if (separation_factor <= 0 || separation_factor > 100) {
      throw std::invalid_argument(
          "separation_factor must be greater than 0 and smaller than 100");
    }

    for (int i = 0; i < size; i++) {
      boids.push_back(boid(
          vec3(0, 0, 0),
          vec3(0, 0, 0))); // Inizialmente posiziona tutti i boids su (0, 0, 0)
    }
    initialize_positions(); // Chiama il metodo per inizializzare le posizioni
  }

  boid& operator[](int i)
  {
    return boids[i];
  }

  void rule4(boid& b) // rimbalzo
  {
    if (toroidal) {
      teleport(b);
    } else {
      bounce(b);
    }
  }

  void update_predator()
  {
    // Aggiorna la posizione e il comportamento del predatore
    attack(batman);
    batman.update_boid(vec3(), batman.get_attack_speed());
    rule4(batman);
  }

  // Metodo per aggiornare la posizione di tutti i boid nello stormo
  void update_swarm()
  {
    vec3 v1, v2, v3, v_evade;
    vec3 new_pos;
    // mass_center = calculate_center_of_mass();

    for (int i = 0; i < size; i++) {
      v1      = rule1(boids[i]);
      v2      = rule2(boids[i]);
      v3      = rule3(boids[i]);
      v_evade = avoid_predator(boids[i]);

      boids[i].update_boid(v1 + v2 + v3 + v_evade, max_speed);
      boids[i].update_boid(wind, wind.norm() + max_speed);

      rule4(boids[i]);
    }
  }

  int get_size()
  {
    return size;
  }

  vec3 get_screen()
  {
    return screen;
  }

  void set_predator(const predator& new_predator)
  {
    batman = new_predator;
  }

  double get_max_speed()
  {
    return max_speed;
  }

  double get_min_distance()
  {
    return min_distance;
  }

  double get_separation_factor()
  {
    return separation_factor;
  }

  double get_cohesion_factor()
  {
    return cohesion_factor;
  }

  double get_allineation_factor()
  {
    return allineation_factor;
  }

  vec3 get_wind()
  {
    return wind;
  }

  int get_wingspan()
  {
    return wingspan;
  }

  std::vector<boid> get_boids()
  {
    return boids;
  }
};

#endif