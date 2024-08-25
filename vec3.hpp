#ifndef VEC3
#define VEC3

#include <cmath>
#include <iostream>

class vec3
{ // vettore tridimensionale
 public:
  double x, y, z;
  vec3()
      : x(0)
      , y(0)
      , z(0)
  {} // costruttore vuoto
  vec3(double x, double y, double z)
      : x(x)
      , y(y)
      , z(z)
  {} // costruttore
  vec3 operator+(const vec3& other) const
  { // somma
    return vec3(x + other.x, y + other.y, z + other.z);
  }
  vec3 operator+=(const vec3& other)
  { // somma e assegna
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
  vec3 operator-(const vec3& other) const
  { // differenza
    return vec3(x - other.x, y - other.y, z - other.z);
  }
  vec3 operator-=(const vec3& other)
  { // differenza e assegna
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }
  vec3 operator*(double scalar) const
  { // prodotto per uno scalare
    return vec3(x * scalar, y * scalar, z * scalar);
  }
  vec3 operator*=(double scalar)
  { // prodotto per uno scalare e assegna
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }
  vec3 operator/(double scalar) const
  { // divisione per uno scalare
    return vec3(x / scalar, y / scalar, z / scalar);
  }
  vec3 operator/=(double scalar)
  { // divisione per uno scalare e assegna
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }
  double norm() const
  { // norma
    return std::sqrt(x * x + y * y + z * z);
  }
  vec3 normalize()
  { // normalizza
    double n = norm();
    if (n != 0) {
      return vec3(x / n, y / n, z / n);
    } else {
      return *this;
    }
  }
  double dot(const vec3& other)
  { // prodotto scalare
    return x * other.x + y * other.y + z * other.z;
  }
  vec3 cross(const vec3& other)
  { // prodotto vettoriale
    return vec3(y * other.z - z * other.y, z * other.x - x * other.z,
                x * other.y - y * other.x);
  }
  double& operator[](int i)
  { // accesso agli elementi
    if (i == 0) {
      return x;
    } else if (i == 1) {
      return y;
    } else if (i == 2) {
      return z;
    } else {
      throw std::out_of_range("index out of range");
    }
  }

  // OPERATORE AGGIUNTO CASUALMENTE DA MIA MAMMA PER STAMPARE NON SO COSA FA
  //  Overload l'operatore << per la stampa con std::cout
  friend std::ostream& operator<<(std::ostream& os, const vec3& v)
  {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
  }
};

#endif