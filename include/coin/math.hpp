#pragma once

#include <iomanip>

namespace coin {

namespace _impl_math {

template<typename T> 
constexpr T min(T a, T b ) { return a > b ? b : a; }

template <typename T>
inline T    min(const T& t) { return t; }

template <typename T, typename ...P>
inline auto min(const T& t, const P& ...p) {
	using res_type= std::common_type_t<T, P...>;
	return std::min(res_type(t), res_type(max(p...)));
}

template<typename T> 
constexpr T max(T a, T b ) { return a < b ? b : a; }

template <typename T>
inline    T max(const T& t) { return t; }

template <typename T, typename ...P>
inline auto max(const T& t, const P& ...p) {
	using res_type= std::common_type_t<T, P...>;
	return std::max(res_type(t), res_type(max(p...)));
}

template<typename T>
constexpr T sum(T x) { return x; }

template<typename T, typename... Ts>
constexpr T sum(T x, Ts... xs) { return x + sum(xs...); }


template<typename T>
constexpr T meta_prod(T x) { return x; }
 
template<typename T, typename... Ts>
constexpr T meta_prod(T x, Ts... xs) { return x * meta_prod(xs...); }

template<typename T, typename E>
constexpr T meta_pow(T base, E expo) { return (expo != 0 ) ? base * meta_pow(base, expo-1) : 1; }

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

using _impl_math::min;
using _impl_math::max;
using _impl_math::sum;
using _impl_math::meta_prod;
using _impl_math::meta_pow;
using _impl_math::abs;
using _impl_math::sqrt;
using _impl_math::deg_to_rad;
using _impl_math::set_max_decimal_digits;


} // namespace coin
