#include <cmath>
#include <cstdint>
#include "./permutations.h"

double fade(double t) {
  return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(
  double t,
  double a,
  double b
) {
  return a + t * (b - a);
}

double grad(
  int32_t hash,
  double x,
  double y,
  double z
) {
  int32_t h = hash & 15;
  double u = h < 8 ? x : y;
  double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
  return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

double noise(
  double x,
  double y,
  double z
) {
  int32_t X = static_cast<int32_t>(std::floor(x)) & 255;
  int32_t Y = static_cast<int32_t>(std::floor(y)) & 255;
  int32_t Z = static_cast<int32_t>(std::floor(z)) & 255;

  x -= std::floor(x);
  y -= std::floor(y);
  z -= std::floor(z);

  double u = fade(x);
  double v = fade(y);
  double w = fade(z);

  int32_t A = p[X] + Y;
  int32_t AA = p[A] + Z;
  int32_t AB = p[A + 1] + Z;
  int32_t B = p[X + 1] + Y;
  int32_t BA = p[B] + Z;
  int32_t BB = p[B + 1] + Z;

  return lerp(
    w, lerp(
      v, lerp(
        u, grad(p[AA], x, y, z),
        grad(p[BA], x - 1, y, z)),
      lerp(
        u, grad(p[AB], x, y - 1, z),
        grad(p[BB], x - 1, y - 1, z))),
    lerp(
      v, lerp(
        u, grad(p[AA + 1], x, y, z - 1),
        grad(p[BA + 1], x - 1, y, z - 1)),
      lerp(
        u, grad(p[AB + 1], x, y - 1, z - 1),
        grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

double perlin3(
  double x,
  double y,
  double z,
  int32_t octaves,
  double persistence
) {
  double total = 0.0;
  double frequency = 1.0;
  double amplitude = 1.0;
  double max_value = 0.0;

  for (int32_t i = 0; i < octaves; i++) {
    total += noise(x * frequency, y * frequency, z * frequency) * amplitude;
    max_value += amplitude;
    amplitude *= persistence;
    frequency *= 2.0;
  }

  return total / max_value;
}

