#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <iostream>
using namespace std;
#include <vector>

class PerlinNoise {
public:
  PerlinNoise() {}
  ~PerlinNoise() {}
  double noise(double x, double y, double z);
  double noise(unsigned int seed, double x, double y, double z);

private:
  double fade(double t);
  double lerp(double t, double a, double b);
  double grad(int hash, double x, double y, double z);
};

#endif // PERLIN_NOISE_H
