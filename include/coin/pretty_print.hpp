#pragma once

#include <sstream>
#include <string>
#include <type_traits>

namespace coin {

namespace _impl_stringify {

template<class ...> using void_t = void;


// for classes which own a `to_string` method
template<typename T, typename = void>
struct has_to_string : std::false_type { };

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


// for std::pair

template<class U> struct is_pair : public std::false_type {};

template<class U, class V> struct is_pair<std::pair<U,V>> : public std::true_type {};

template<typename U, typename V>
std::string to_string(const std::pair<U,V>& p); // must declare it because of (1)

template<typename T> std::enable_if_t<!is_pair<T>::value, std::string>
stringify_bis(const T& t) { return t; }

template<typename T> std::enable_if_t<is_pair<T>::value, std::string>
stringify_bis(const T& t) { return to_string(t); } // (1)

template<typename T> std::enable_if_t<!std::is_fundamental<T>::value, std::string>
stringify(const T& t) { return stringify_bis(t); }

template<typename T> std::enable_if_t<std::is_fundamental<T>::value, std::string>
stringify(const T& t) { return std::to_string(t); }

template<typename U, typename V>
std::string to_string(const std::pair<U,V>& p) {
	using std::to_string;
	return std::string{"{" + stringify(p.first) + ":" + stringify(p.second) + "}"};
}


// for nested containers

template<class T> struct is_container : public std::false_type {};

template<class T, class Alloc> 
struct is_container<std::vector<T, Alloc>> : public std::true_type {};

template<class K, class T, class Comp, class Alloc> 
struct is_container<std::map<K, T, Comp, Alloc>> : public std::true_type {};


template <typename Cont, std::enable_if_t<!is_container<typename Cont::value_type>::value>* = nullptr>
std::string to_string(Cont const& container) {
	using std::to_string;
	std::string str = "[";
	for (auto &&el : container) { str += stringify(el) + ";"; }
	str += "\b]";
	return str;
}

template <typename Cont, std::enable_if_t<is_container<typename Cont::value_type>::value>* = nullptr>
std::string to_string(Cont const& container) {
	std::string str = "[";
	for (auto &&el : container) { str += to_string(el) + ";"; }
	str += "\b]";
	return str;
}



// magic operates : overload ostream operator<< !

template<class T, class Alloc> 
std::ostream& operator<<(std::ostream& os, const std::vector<T, Alloc>& v) {
	using std::operator<<;
	os << to_string(v);
	return os;
}


template<class T, std::size_t N> 
std::ostream& operator<<(std::ostream& os, const std::array<T, N>& arr) {
	using std::operator<<;
	os << to_string(arr);
	return os;
}


template<class Key, class T, class Comp, class Alloc> 
std::ostream& operator<<(std::ostream& os, const std::map<Key,T,Comp,Alloc>& m) {
	using std::operator<<;
	os << to_string(m);
	return os;
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

