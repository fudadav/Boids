#ifndef FUNCTIONS
#define FUNCTIONS

#include "vec3.hpp"

#include <numeric>
#include <vector>

vec3 toroidal_vec_dist(const vec3& a, const vec3& b,
                       const vec3& width) // va da b ad a
{
  double dx = a.x - b.x;
  double dy = a.y - b.y;
  double dz = a.z - b.z;

  if (std::abs(dx) > width.x / 2) {
    if (a.x > b.x) {
      dx -= width.x;
    } else {
      dx += width.x;
    }
  }
  if (std::abs(dy) > width.y / 2) {
    if (a.y > b.y) {
      dy -= width.y;
    } else {
      dy += width.y;
    }
  }
  if (std::abs(dz) > width.z / 2) {
    if (a.z > b.z) {
      dz -= width.z;
    } else {
      dz += width.z;
    }
  }

  vec3 dist(dx, dy, dz);

  return dist;
}

double toroidal_distance(const vec3& a, const vec3& b, const vec3& width)
{
  double dx = std::abs(a.x - b.x);
  double dy = std::abs(a.y - b.y);
  double dz = std::abs(a.z - b.z);

  if (dx > width.x / 2) {
    dx -= width.x;
  }
  if (dy > width.y / 2) {
    dy -= width.y;
  }
  if (dz > width.z / 2) {
    dz -= width.z;
  }

  return std::sqrt(dx * dx + dy * dy + dz * dz);
}

double distance(const vec3& a, const vec3& b)
{
  return (a - b).norm();
}

double mean(std::vector<double> const& v)
{
  return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

double dev_std(std::vector<double> const& v, double mean)
{
  double sum =
      std::accumulate(v.begin(), v.end(), 0.0, [mean](double a, double b) {
        return a + (b - mean) * (b - mean);
      });
  return std::sqrt(sum / v.size());
}

#endif