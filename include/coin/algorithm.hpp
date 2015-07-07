#pragma once

namespace coin {

namespace _impl_algorithm {

template<class Container, class Functor>
void generate(Container cont, Functor func) {
	using std::begin;
	using std::endl;
	std::generate(begin(cont), end(cont), func);
}

template<typename T> 
constexpr T min(T a, T b ) { return a > b ? b : a; }

template <typename T>
inline T min(const T& t) { return t; }

template <typename T, typename ...P>
inline auto min(const T& t, const P& ...p) {
	using res_type= std::common_type_t<T, P...>;
	return std::min(res_type(t), res_type(max(p...)));
}

template<typename T> 
constexpr T max(T a, T b ) { return a < b ? b : a; }

template <typename T>
inline T max(const T& t) { return t; }

template <typename T, typename ...P>
inline auto max(const T& t, const P& ...p) {
	using res_type= std::common_type_t<T, P...>;
	return std::max(res_type(t), res_type(max(p...)));
}

template<typename T>
constexpr T sum(T x) { return x; }

template<typename T, typename... Ts>
T sum(T x, Ts... xs) { return x + sum(xs...); }

} // namespace _impl_algorithm

using _impl_algorithm::generate;
using _impl_algorithm::min;
using _impl_algorithm::max;
using _impl_algorithm::sum;


} // namespace coin

