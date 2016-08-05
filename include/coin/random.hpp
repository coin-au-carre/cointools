#pragma once

#include <random>
#include <algorithm>

namespace coin {


namespace _detail {

template <typename T>
using uniform_generic_distribution_t = std::conditional_t<
  std::is_floating_point<T>::value,
  std::uniform_real_distribution<T>,
  std::uniform_int_distribution<T>
>;


template<typename IntType>
inline
auto max_uniform_param() 
-> typename std::enable_if<std::is_integral<IntType>::value, IntType>::type {
    return std::numeric_limits<IntType>::max();
}

template<typename RealType>
inline
auto max_uniform_param()
-> typename std::enable_if<std::is_floating_point<RealType>::value, RealType>::type {
    return 1.0;
}


template<class Container, class UniformRandomNumberGenerator>
inline
void fill_random_uniform(
    Container& container, 
    UniformRandomNumberGenerator&& g,
    typename Container::value_type min = 0, 
    typename Container::value_type max = max_uniform_param<typename Container::value_type>()
) {
  using std::begin; using std::end;
  uniform_generic_distribution_t<typename Container::value_type> distribution(min, max);
  auto lambda_norm_dist = [&distribution, &g]() { return distribution(g); };
  std::generate(begin(container), end(container), lambda_norm_dist);
}

inline
auto mt19937_generator() -> std::mt19937 {
    std::random_device rd;
    std::mt19937 rng{rd()};
    return rng;
}

inline
auto default_random_generator() -> std::default_random_engine {
    auto rd  = std::chrono::system_clock::now().time_since_epoch().count();
    auto rng = std::default_random_engine{rd};
    return rng;
}

} // ns _detail

using _detail::uniform_generic_distribution_t;
using _detail::fill_random_uniform;
using _detail::mt19937_generator;
using _detail::default_random_generator;


} // ns coin

