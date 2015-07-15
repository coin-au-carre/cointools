#pragma once

#include <limits>

namespace coin {

namespace _impl_constant {

template<typename T>
constexpr T k_infinity = std::numeric_limits<T>::infinity();

template<typename T>
constexpr T k_epsilon  = std::numeric_limits<T>::epsilon();

template<typename T>
constexpr T k_min      = std::numeric_limits<T>::min();

template<typename T>
constexpr T k_lower    = std::numeric_limits<T>::lower();

template<typename T>
constexpr T k_max      = std::numeric_limits<T>::max();

#if defined(__clang__)

template<typename T>
constexpr T pi() { return 3.141592653589793238463; }

#elif defined(__GNUC__) || defined(__GNUG__)
  
template<typename T>
constexpr T pi() { return std::atan(1)*4; } // non standard but GCC permits it

#endif

} // namespace _impl_constant

using _impl_constant::k_infinity;
using _impl_constant::k_epsilon;
using _impl_constant::k_min;
using _impl_constant::k_lower;
using _impl_constant::k_max;
using _impl_constant::pi;

} // namespace coin

