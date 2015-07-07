#pragma once

#include <sstream>
#include <string>
#include <type_traits>

namespace coin {

namespace _impl_stringify {

template<class ...> using void_t = void;

// for nested containers
template <typename T>
struct has_cont {
	static const bool value = false;
};

template <typename T,typename Alloc>
struct has_cont<std::vector<T,Alloc>> {
	static const bool value = true;
};

template <typename T, std::enable_if_t<!has_cont<typename T::value_type>::value>* = nullptr>
std::string to_string(T const& container) {
	using std::to_string;
	std::string str = "[";
	for (auto &&el : container) { str += to_string(el) + ";"; }
	str += "\b]";
	return str;
}

template <typename T, std::enable_if_t<has_cont<typename T::value_type>::value>* = nullptr>
std::string to_string(T const& container) {
	std::string str = "[";
	for (auto &&el : container) { str += to_string(el) + ";"; }
	str += "\b]";
	return str;
}

// for classes which own a `to_string` method
template<typename T, typename = void>
struct has_to_string
: std::false_type { };

template<typename T>
struct has_to_string<T, 
	void_t<decltype(std::declval<T>().to_string())>
	>
	: std::true_type { };

template<typename T> std::enable_if_t<has_to_string<T>::value, std::ostream&>
operator<<(std::ostream& os, const T& t) {
	os << t.to_string();
	return os;
}

/*
template<typename T> std::enable_if_t<!has_to_string<T>::value, std::ostream&>
operator<<(std::ostream& os, const T& t) {
	os << t;
	return os;
}


std::ostream& operator<<(std::ostream& os, const std::string& t) {
	using std::operator<<;
	os << t;
	return os;
}

std::ostream& operator<<(std::ostream& os, const char* t) {
	using std::operator<<;
	os << t;
	return os;
}
*/

// magic operates !

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
	using std::operator<<;
	os << to_string(v);
	return os;
}

template<typename U, typename V>
std::string to_string(const std::pair<U,V>& p) {
	using std::to_string;
	return std::string{"{" + to_string(p.first) + ";" + to_string(p.second) + "}"};
}

template<typename U, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<U,V>& p) {
	using std::operator<<;
	os << to_string(p);
	return os;
}


} // namespace _impl_stringify

using _impl_stringify::to_string; // for container
using _impl_stringify::operator<<;

} // namespace coin

