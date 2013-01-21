// -----------------------------------------------------------------------------
// Perlin Noise
// Copyright (C) 2010 Ares Lagae (ares.lagae@gmail.com)
// -----------------------------------------------------------------------------

#ifndef PERLIN_NOISE_HPP_INCLUDED
#define PERLIN_NOISE_HPP_INCLUDED

#include <cassert>
#include <cmath>
#include <cstddef>

class rand_urng
{
public:
  typedef unsigned int result_type;
  static const result_type min() { return 0; }
  static const result_type max() { return RAND_MAX; }
  typedef unsigned int seed_type;
  static const seed_type default_seed = 1;
  rand_urng(seed_type s = default_seed) { seed(s); }
  void seed(seed_type s = default_seed) { std::srand(s); }
  result_type operator()()
  {
    result_type result = std::rand();
    assert((min() <= result) && (result <= max()));
    return result;
  }
};

namespace perlin_noise {

// -----------------------------------------------------------------------------

class perlin_noise_permutation_table
{
public:
  template <typename UniformRandomNumberGenerator>
  perlin_noise_permutation_table(UniformRandomNumberGenerator& urng)
    : data_(0)
  {
    data_ = new unsigned char[256];
    randomize(urng);
  }
  ~perlin_noise_permutation_table() { delete [] data_; }
  const unsigned char* data() const { return data_; }
  template <typename UniformRandomNumberGenerator>
  void randomize(UniformRandomNumberGenerator& urng)
  {
    for (unsigned i = 0; i < 256; ++i) {
      data_[i] = i;
    }
    for (unsigned i = 0; i < 256 - 1; ++i) {
      std::swap(data_[i], data_[i + (urng() % (256 - i))]);
    }
  }
  const unsigned char& at(unsigned index) const
  {
    assert(index < 256);
    return data_[index];
  }
  const unsigned char& operator[](unsigned index) const { return at(index); }
private:
  perlin_noise_permutation_table(const perlin_noise_permutation_table&);
  perlin_noise_permutation_table& operator=(const perlin_noise_permutation_table&);
  unsigned char* data_;
};

// -----------------------------------------------------------------------------

class perlin_noise_2d
{
public:
  template <typename UniformRandomNumberGenerator>
  perlin_noise_2d(UniformRandomNumberGenerator& urng)
    : permutation_table_(urng)
  {}
  template <typename UniformRandomNumberGenerator>
  void randomize(UniformRandomNumberGenerator& urng) { permutation_table_.randomize(urng); }
  float operator()(float x_0, float x_1) const { return noise(x_0, x_1); }
  float noise(float x_0, float x_1) const
  {
    float x[2] = { x_0, x_1 };
    float int_x[2] = { std::floor(x[0]), std::floor(x[1]) };
    unsigned c[2] = { unsigned(int(int_x[0])) % 256u, unsigned(int(int_x[1])) % 256u };
    float x_c[2] = { x[0] - int_x[0], x[1] - int_x[1] };
    float w[2] = { ((((6.0f * x_c[0]) - 15.0f) * x_c[0]) + 10.0f) * (x_c[0] * x_c[0] * x_c[0]), ((((6.0f * x_c[1]) - 15.0f) * x_c[1]) + 10.0f) * (x_c[1] * x_c[1] * x_c[1]) };
    float noise[2][2];
    unsigned i[2];
    for (i[1] = 0u; i[1] < 2u; ++i[1]) {
      unsigned hash_1 = permutation_table_[(c[1] + i[1]) % 256u];
      for (i[0] = 0u; i[0] < 2u; ++i[0]) {
        unsigned hash_0 = permutation_table_[(hash_1 + (c[0] + i[0])) % 256u];
        float v[2] = { x_c[0] - i[0], x_c[1] - i[1] };
        noise[i[1]][i[0]] = grad(hash_0, v);
      }
    }
    return mix(mix(noise[0][0], noise[0][1], w[0]),
               mix(noise[1][0], noise[1][1], w[0]), w[1]);
  }
  const perlin_noise_permutation_table& permutation_table() const { return permutation_table_; }
  float variance() const { return 0.0628; }
private:
  static float mix(float x, float y, float a) { return x + (a * (y - x)); }
  static float grad(unsigned hash, const float x[2])
  {
    unsigned h = hash & 15u;                      
    float u = (h < 8u) ? x[0] : x[1];
    float v = (h < 4u) ? x[1] : (((h == 12u) || (h == 14u)) ? x[0] : 0.0);
    return ((h & 1u) == 0u ? u : -u) + ((h & 2u) == 0u ? v : -v);
  }
  perlin_noise_permutation_table permutation_table_;
};

// -----------------------------------------------------------------------------

class perlin_noise_3d
{
public:
  template <typename UniformRandomNumberGenerator>
  perlin_noise_3d(UniformRandomNumberGenerator& urng)
    : permutation_table_(urng)
  {}
  template <typename UniformRandomNumberGenerator>
  void randomize(UniformRandomNumberGenerator& urng) { permutation_table_.randomize(urng); }
  float operator()(float x_0, float x_1, float x_2) const { return noise(x_0, x_1, x_2); }
  float noise(float x_0, float x_1, float x_2) const
  {
    float x[3] = { x_0, x_1, x_2 };
    float int_x[3] = { std::floor(x[0]), std::floor(x[1]), std::floor(x[2]) };
    unsigned c[3] = { unsigned(int(int_x[0])) % 256u, unsigned(int(int_x[1])) % 256u, unsigned(int(int_x[2])) % 256u };
    float x_c[3] = { x[0] - int_x[0], x[1] - int_x[1], x[2] - int_x[2] };
    float w[3] = { ((((6.0f * x_c[0]) - 15.0f) * x_c[0]) + 10.0f) * (x_c[0] * x_c[0] * x_c[0]), ((((6.0f * x_c[1]) - 15.0f) * x_c[1]) + 10.0f) * (x_c[1] * x_c[1] * x_c[1]), ((((6.0f * x_c[2]) - 15.0f) * x_c[2]) + 10.0f) * (x_c[2] * x_c[2] * x_c[2]) };    
    float noise[2][2][2];
    unsigned i[2];
    for (i[2] = 0u; i[2] < 2u; ++i[2]) {
      unsigned hash_2 = permutation_table_[(c[2] + i[2]) % 256u];
      for (i[1] = 0u; i[1] < 2u; ++i[1]) {
        unsigned hash_1 = permutation_table_[(hash_2 + (c[1] + i[1])) % 256u];
        for (i[0] = 0u; i[0] < 2u; ++i[0]) {
          unsigned hash_0 = permutation_table_[(hash_1 + (c[0] + i[0])) % 256u];
          float v[3] = { x_c[0] - i[0], x_c[1] - i[1], x_c[2] - i[2] };
          noise[i[2]][i[1]][i[0]] = grad(hash_0, v);
        }
      }
    }
    return mix(mix(mix(noise[0][0][0], noise[0][0][1], w[0]),
                   mix(noise[0][1][0], noise[0][1][1], w[0]), w[1]),
               mix(mix(noise[1][0][0], noise[1][0][1], w[0]),
                   mix(noise[1][1][0], noise[1][1][1], w[0]), w[1]), w[2]);
  }
  const perlin_noise_permutation_table& permutation_table() const { return permutation_table_; }
  float variance() const { return 0.0754198; }  
private:
  static float mix(float x, float y, float a) { return x + (a * (y - x)); }
  static float grad(unsigned hash, const float x[3])
  {
    unsigned h = hash & 15u;                      
    float u = (h < 8u) ? x[0] : x[1];
    float v = (h < 4u) ? x[1] : (((h == 12u) || (h == 14u)) ? x[0] : x[2]);
    return ((h & 1u) == 0u ? u : -u) + ((h & 2u) == 0u ? v : -v);
  }
  perlin_noise_permutation_table permutation_table_;
};

// -----------------------------------------------------------------------------

} // namespace perlin_noise

#endif // PERLIN_NOISE_HPP_INCLUDED

// -----------------------------------------------------------------------------
