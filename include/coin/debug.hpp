#pragma once

#include "preprocessor.hpp"

#include <iostream>
#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>
#include <cassert>

#ifndef NDEBUG
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif
#define coin_debug_print(msg, ...) \
	do { fprintf(stdout, "[debug] " msg "\n", ##__VA_ARGS__); } while(0)
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#define coin_debug_info()          \
	do { std::cout << "[debug] " << __FUNCTION__ << \
	"() in " << __FILE__ << ":" << __LINE__ << std::endl; } while(0)
#define coin_debug_to_string()     \
	do { std::cout << "[debug] to_string() : " << to_string() << std::endl; } while(0)
#define coin_assert(x,msg) do { assert(x && msg); } while(0)
#else
#define coin_debug_print(msg, ...)
#define coin_debug_info(msg)
#define coin_debug_to_string()	
#define coin_assert(msg,x)
#endif

namespace coin {

namespace _impl_debug {

// output function that requires a compile-time constant for testing
template<int N> struct CheckCompileTime {
	CheckCompileTime() { std::cout << N << std::endl; }
};

template <class T>
std::string type_name() {
	typedef typename std::remove_reference<T>::type TR;
	std::unique_ptr<char, void(*)(void*)> own (
#ifndef _MSC_VER
		abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr),
#else
		nullptr,
#endif
		std::free
	);
	std::string r = own != nullptr ? own.get() : typeid(TR).name();
	if (std::is_const<TR>::value)
		r += " const";
	if (std::is_volatile<TR>::value)
		r += " volatile";
	if (std::is_lvalue_reference<T>::value)
		r += "&";
	else if (std::is_rvalue_reference<T>::value)
		r += "&&";
	return r;
}

} // namespace _impl_debug

using _impl_debug::CheckCompileTime;
using _impl_debug::type_name;

} // namespace coin
