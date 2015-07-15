#pragma once

#include <iomanip>

namespace coin {

namespace _impl_math {

template<class T>
constexpr T abs(T x){
	return x < 0 ? -x : x;
}

template<typename T>
constexpr T sqrt(const T x, const T epsilon = k_epsilon<T>) {
	T s = x, s0 = 1.0;
	while (abs(s - s0) > epsilon) {
		s0 = s;
		s = 0.5 * (s + x/s);
	}
	return s;
}

template<typename T>
constexpr T deg_to_rad(const T deg) {
	return deg * pi<T>() / 180.0;
}

template<typename T>
void set_max_decimal_digits() {
	std::cout << std::fixed
	<< std::setprecision(std::numeric_limits<T>::digits10);
}

} // namespace _impl_math

using _impl_math::abs;
using _impl_math::sqrt;
using _impl_math::deg_to_rad;
using _impl_math::set_max_decimal_digits;


} // namespace coin
