#pragma once

#include <cmath>
#include <iomanip>
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

template<typename T>
constexpr T deg_to_rad(T deg) {
	return deg * pi<T>() / 180.0;
}

template<typename T>
void set_max_decimal_digits() {
	std::cout << std::fixed
	<< std::setprecision(std::numeric_limits<T>::digits10);
}


} // namespace _impl_constant

using _impl_constant::k_infinity;
using _impl_constant::k_epsilon;
using _impl_constant::k_min;
using _impl_constant::k_lower;
using _impl_constant::k_max;
using _impl_constant::pi;
using _impl_constant::deg_to_rad;
using _impl_constant::set_max_decimal_digits;

} // namespace coin

