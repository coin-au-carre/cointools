#pragma once

#include <algorithm>
#include <vector>
#include <map>
#include <utility>
#include <cassert>

namespace coin {

namespace _impl_algorithm {

template<typename T, size_t Size> T* begin(T (& array)[Size]) { return array; } 
template<typename T, size_t Size> T* end  (T (& array)[Size]) { return array + Size; } 

template<typename T, size_t N>
constexpr size_t get_size_of_array(T(&)[N]) {
	return N;
}

// or call std::vector<T>(data, get_size_of_array(data));
template< typename T, size_t N>
std::vector<T> make_vector_from_array(const T (&data)[N]) {
	return std::vector<T>(data, data+N);
}

template<typename Container>
void remove_duplicate(Container& cont) {
	using std::begin; using std::end;
	std::sort(begin(cont), end(cont));
	cont.erase(unique(begin(cont), end(cont)), end(cont));
}

template<typename T, typename Alloc>
void remove_element(std::vector<T,Alloc> vec, const T& element) {
	vec.erase(std::remove(vec.begin(), vec.end(), element), vec.end());
}

template<class K, class T, class Comp, class Alloc> 
void remove_element(std::map<K,T,Comp,Alloc>& m, const std::pair<K,T>& p) {
	typename std::map<K,T,Comp,Alloc>::iterator it = m.begin();
	while (it != m.end()) {
		if (it->first == p.first && it->second == p.second) {
			it = m.erase(it);
		} 
		else {
			++ it;
		}
	}
}

template<class K, class T, class Comp, class Alloc> 
void remove_element(std::map<K,T,Comp,Alloc>& m, const K& k) {
	typename std::map<K,T,Comp,Alloc>::iterator it = m.begin();
	while (it != m.end()) {
		if (it->first == k) {
			it = m.erase(it);
		} 
		else {
			++ it;
		}
	}
}

template<class ForwardIt, class T>
T lower_bound_index(ForwardIt first, ForwardIt last, const T& value) {
	return (std::lower_bound (first, last, value) - first);
}

template<class ForwardIt, class T>
T upper_bound_index(ForwardIt first, ForwardIt last, const T& value) {
    return (std::upper_bound (first, last, value) - first);
}


template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
bool multi_dim_counter(std::vector<T>& v, const std::vector<T>& lower, const std::vector<T>& upper) {
	assert(v.size() == lower.size());
	assert(v.size() == upper.size());
	for (auto i = v.size(); i-- != 0; ) {
		++ v[i];
		if (v[i] != upper[i] + 1) {
			return true;
		}
		v[i] = lower[i];
	}
	return false;
}


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
T sum(T x, Ts... xs) { return x + sum(xs...); }

} // namespace _impl_algorithm

using _impl_algorithm::begin;
using _impl_algorithm::end;
using _impl_algorithm::get_size_of_array;
using _impl_algorithm::remove_duplicate;
using _impl_algorithm::remove_element;
using _impl_algorithm::lower_bound_index;
using _impl_algorithm::upper_bound_index;
using _impl_algorithm::multi_dim_counter;
using _impl_algorithm::min;
using _impl_algorithm::max;
using _impl_algorithm::sum;


} // namespace coin

