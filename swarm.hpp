#ifndef SWARM
#define SWARM

#include "boid.hpp"
#include "functions.hpp"
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
  double sight_distance;
  double separation_factor;  // si allontana della distanza tra i boid *
                             // separation_factor
  double cohesion_factor;    // si avvicina di quella percentuale da 1 a 100 al
                             // centro di massa
  double allineation_factor; // si avvicina alla velocità generale di un
                             // ottavo della differenza tra la velocità
                             // percepita e la propria * allineation_factor
  double fear_factor;        // quanto velocemente scappano i boids
  bool predator;
  boid batman;
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

      b.set_velocity(vec3(vx, vy, vz).normalize()
                     * max_speed); // Imposta la velocità iniziale del boid
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
    int count{};
    if (!toroidal) { // bordo
      for (int i = 0; i < size; i++) {
        if (boids[i] != b) {
          if (distance(boids[i].get_position(), b.get_position())
              <= sight_distance) { // non considero i boid troppo lontani
            perceived_center += boids[i].get_position();
            ++count;
          }
        }
      }
      if (count == 0) {
        return vec3(0, 0, 0);
      }
      perceived_center /= count;
      return ((perceived_center - b.get_position()) / 100000) * cohesion_factor;
    } else { // toroidale
      for (int i = 0; i < 3; ++i) {
        count = 0;
        for (int j = 0; j < size; ++j) {
          if (boids[j] != b
              && toroidal_distance(boids[j].get_position(), b.get_position(),
                                   screen)
                     <= sight_distance) {
            if (std::abs(boids[j].get_position()[i] - perceived_center[i])
                < screen[i] / 2) {
              perceived_center[i] =
                  (count * perceived_center[i] + boids[j].get_position()[i])
                  / (count + 1);
            } else {
              perceived_center[i] = (count * perceived_center[i]
                                     + boids[j].get_position()[i] + screen[i])
                                  / (count + 1);
              if (perceived_center[i] > screen[i]) {
                perceived_center[i] -= screen[i];
              }
            }
            ++count;
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
        if (boids[i] != b
            && distance(boids[i].get_position(), b.get_position()) != 0) {
          if (distance(boids[i].get_position(), b.get_position())
              <= min_distance) {
            c -= (boids[i].get_position() - b.get_position()).normalize()
               / ((boids[i].get_position() - b.get_position()).norm());
          }
        }
      }
    } else { // toroidale
      for (int i = 0; i < size; i++) {
        if (boids[i] != b
            && distance(boids[i].get_position(), b.get_position()) != 0) {
          if (toroidal_distance(boids[i].get_position(), b.get_position(),
                                screen)
              <= min_distance) {
            c -= toroidal_vec_dist(boids[i].get_position(), b.get_position(),
                                   screen)
                     .normalize()
               / (toroidal_vec_dist(boids[i].get_position(), b.get_position(),
                                    screen)
                      .norm());
          }
        }
      }
    }
    return c * separation_factor / 100;
  }

  vec3 rule3(boid& b) // tutti i boid tendono a muoversi nella stessa direzione
  {
    vec3 pv(0, 0, 0);
    int count = 0;
    if (!toroidal) {
      for (int i = 0; i < size; i++) {
        if (boids[i] != b
            && distance(boids[i].get_position(), b.get_position())
                   <= sight_distance) {
          pv += boids[i].get_velocity();
          ++count;
        }
      }
    }
    if (toroidal) {
      for (int i = 0; i < size; i++) {
        if (boids[i] != b
            && toroidal_distance(boids[i].get_position(), b.get_position(),
                                 screen)
                   <= sight_distance) {
          pv += boids[i].get_velocity();
          ++count;
        }
      }
    }
    if (count == 0) {
      return vec3(0, 0, 0);
    }
    pv /= count;
    return (pv - b.get_velocity()) / 1000 * allineation_factor;
  }

  void bounce(boid& b) // rimbalzo
  {
    vec3 bounce = b.get_velocity();
    vec3 newpos = b.get_position();
    for (int i = 0; i < 3; i++) {
      if (b.get_position()[i] < 0 && b.get_velocity()[i] < 0) {
        bounce[i] *= -0.9;
      } else if (b.get_position()[i] > screen[i] && b.get_velocity()[i] > 0) {
        bounce[i] *= -0.9;
      }
      if (b.get_position()[i] < 0) {
        newpos[i] = 0;
      } else if (b.get_position()[i] > screen[i]) {
        newpos[i] = screen[i];
      }
    }
    b.set_velocity(bounce);
    b.set_position(newpos);
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

  void rule4(boid& b) // rimbalzo
  {
    if (toroidal) {
      teleport(b);
    } else {
      bounce(b);
    }
  }

  vec3 avoid_predator(boid& b) // evita il predatore
  {
    vec3 evade_vector(0, 0, 0);
    vec3 distance_to_predator(0, 0, 0);
    if (!toroidal) {
      distance_to_predator = b.get_position() - batman.get_position();
    } else {
      distance_to_predator =
          toroidal_vec_dist(b.get_position(), batman.get_position(), screen);
    }
    double distance_norm = distance_to_predator.norm();
    if (distance_norm == 0) {
      evade_vector = b.get_velocity() * max_speed;
    } else if (distance_norm <= sight_distance) {
      vec3 flee_direction  = distance_to_predator.normalize();
      double bounce_factor = (sight_distance - distance_norm) / sight_distance;
      evade_vector         = flee_direction * max_speed * bounce_factor;
    }
    return evade_vector * fear_factor / 100;
  }

  vec3 avoid_obstacle(boid& b,
                      boid& batman) // evita un ostacolo mobile circolare
  {
    if (distance(b.get_position(), batman.get_position()) == 0) {
      return b.get_velocity().normalize() * max_speed;
    } else {
      vec3 current_velocity =
          b.get_velocity(); // Ottieni la velocità corrente del boid
      vec3 distance_to_predator = b.get_position() - batman.get_position();
      if (toroidal) {
        distance_to_predator =
            toroidal_vec_dist(b.get_position(), batman.get_position(), screen);
      }
      double distance_norm = distance_to_predator.norm();

      // Se il boid è entro il raggio del predatore (zona da evitare)
      if (distance_norm <= sight_distance) {
        // Calcola la direzione opposta per allontanarsi dal predatore
        vec3 flee_direction = distance_to_predator.normalize();

        // Imposta la velocità del boid verso la direzione opposta al predatore
        vec3 new_velocity = flee_direction * max_speed;

        return new_velocity; // Restituisci la nuova velocità, orientata lontano
                             // dal predatore
      }

      // Se il boid non è in prossimità del predatore, mantiene la velocità
      // corrente
      return current_velocity;
    }
  }

 public:
  swarm(int size_, int wingspan_,
        double max_speed_    = 50, // la maxspeed compare 2 volte
        double min_distance_ = 10, double sight_distance_ = 300,
        double separation_factor_ = 1.5, double cohesion_factor_ = 0.5,
        double allineation_factor_ = 1, double fear_factor_ = 0.5,
        boid batman_ = boid(vec3(0, 0, 0), vec3(0, 0, 0)),
        vec3 screen_ = vec3(500, 500, 500), bool toroidal_ = 0,
        vec3 wind_ = vec3(0, 0, 0))
      : size(size_)
      , wingspan(wingspan_)
      , max_speed(max_speed_)
      , min_distance(min_distance_)
      , sight_distance(sight_distance_)
      , separation_factor(separation_factor_)
      , cohesion_factor(cohesion_factor_)
      , allineation_factor(allineation_factor_)
      , fear_factor(fear_factor_)
      , batman(batman_)
      , screen(screen_) // Imposta larghezza e altezza dello schermo
      , toroidal(toroidal_)
      , wind(wind_)
  {
    if (size < 1) {
      throw std::invalid_argument("Swarm size must be greater than 0");
    }
    if (wingspan <= 0) {
      throw std::invalid_argument("wingspan must be greater than 0");
    }
    if (max_speed <= 0) {
      throw std::invalid_argument("Maximum speed must be greater than 0");
    }
    if (min_distance <= 0) {
      throw std::invalid_argument("Minimum distance must be greater than 0");
    }
    if (allineation_factor <= 0 || allineation_factor > 100) {
      throw std::invalid_argument(
          "Allineation_factor must be greater than 0 and smaller than 100");
    }
    if (cohesion_factor <= 0
        || cohesion_factor > 100) { // è l'unico per cui 100 va bene!!!!!!!!1
      throw std::invalid_argument(
          "Cohesion factor must be greater than 0 and smaller then 100");
    }
    if (separation_factor <= 0 || separation_factor > 100) {
      throw std::invalid_argument(
          "Ceparation factor must be greater than 0 and smaller than 100");
    }

    if (fear_factor <= 0 || separation_factor > 100) {
      throw std::invalid_argument(
          "Fear factor must be greater than 0 and smaller than 100");
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

  // Metodo per aggiornare la posizione di tutti i boid nello stormo
  void update_swarm(boid& batman_)
  {
    batman = batman_;
    vec3 v1, v2, v3, v_evade, v_avoid;
    // mass_center = calculate_center_of_mass();

    for (int i = 0; i < size; i++) {
      v1 = rule1(boids[i]);
      v2 = rule2(boids[i]);
      v3 = rule3(boids[i]);

      boids[i].update_boid(v1 + v2 + v3, max_speed);
      boids[i].update_boid(avoid_predator(boids[i]), max_speed);
      // boids[i].set_velocity(v_evade);
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

  bool is_toroidal()
  {
    return toroidal;
  }
};

#endif