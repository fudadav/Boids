#ifndef VEC3
#define VEC3

#include <cmath>
#include <stdexcept>

class vec3
{ // vettore tridimensionale
public:
  double x, y, z;
  vec3()
      : x(0), y(0), z(0)
  {
  } // costruttore vuoto
  vec3(double x, double y, double z)
      : x(x), y(y), z(z)
  {
  } // costruttore
    
  // uguaglianza
  bool operator==(const vec3 &other) const
  {
    return x == other.x && y == other.y && z == other.z;
  }

  // disuguaglianza
  bool operator!=(const vec3 &other) const
  {
    return !(*this == other);
  }

  // somma
  vec3 operator+(const vec3 &other) const
  {
    return vec3(x + other.x, y + other.y, z + other.z);
  }

  // somma e assegna
  vec3 operator+=(const vec3 &other)
  {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  // differenza
  vec3 operator-(const vec3 &other) const
  {
    return vec3(x - other.x, y - other.y, z - other.z);
  }

  // differenza e assegna
  vec3 operator-=(const vec3 &other)
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  // prodotto per uno scalare
  vec3 operator*(double scalar) const
  {
    return vec3(x * scalar, y * scalar, z * scalar);
  }

  // prodotto per uno scalare e assegna
  vec3 operator*=(double scalar)
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  // divisione per uno scalare
  vec3 operator/(double scalar) const
  {
    return vec3(x / scalar, y / scalar, z / scalar);
  }

  // divisione per uno scalare e assegna
  vec3 operator/=(double scalar)
  {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }

  // norma
  double norm() const
  {
    return std::sqrt(x * x + y * y + z * z);
  }

  // normalizza
  vec3 normalize() const
  {
    double n = norm();
    if (n != 0)
    {
      return vec3(x / n, y / n, z / n);
    }
    else
    {
      return *this;
    }
  }

  // prodotto scalare
  double dot(const vec3 &other) const
  {
    return x * other.x + y * other.y + z * other.z;
  }

  // prodotto vettoriale
  vec3 cross(const vec3 &other) const
  {
    return vec3(y * other.z - z * other.y, z * other.x - x * other.z,
                x * other.y - y * other.x);
  }

  // accesso agli elementi
  double &operator[](int i)
  {
    if (i == 0)
      return x;
    if (i == 1)
      return y;
    if (i == 2)
      return z;
    throw std::out_of_range("index out of range");
  }

  // accesso agli elementi costante
  const double &operator[](int i) const
  {
    if (i == 0)
      return x;
    if (i == 1)
      return y;
    if (i == 2)
      return z;
    throw std::out_of_range("index out of range");
  }
};

#endif