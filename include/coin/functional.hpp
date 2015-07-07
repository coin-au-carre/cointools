#pragma once 

#include <memory>
#include <vector>
#include <algorithm>

namespace coin {

namespace _impl_functional {

template<typename T, size_t Size> T* begin(T (& array)[Size]) { return array; } 
template<typename T, size_t Size> T* end  (T (& array)[Size]) { return array + Size; } 

template<typename T, size_t N>
constexpr size_t get_size_of_array(T(&)[N]) {
	return N;
}

template<class Container>
void remove_duplicate(Container& cont) {
	using std::begin; using std::end;
	std::sort(begin(cont), end(cont));
	cont.erase(unique(begin(cont), end(cont)), end(cont));
}

template <class T>
auto make_shared_vector(const std::vector<T>& v) {
	std::vector<std::shared_ptr<T>> v_shared_ptr;
	v_shared_ptr.reserve(v.size());
	for (auto const& el : v) { 
		v_shared_ptr.push_back(std::make_shared<T>(el));
	}
	return v_shared_ptr;
}

template <class T>
auto make_unique_vector(const std::vector<T>& v) {
	std::vector<std::unique_ptr<T>> v_smart_ptr;
	v_smart_ptr.reserve(v.size());
	for (auto const& el : v) { 
		v_smart_ptr.push_back(std::make_unique<T>(el));
	}
	return v_smart_ptr;
}


} // namespace _impl_functional

using _impl_functional::make_shared_vector;
using _impl_functional::make_unique_vector;
using _impl_functional::remove_duplicate;
using _impl_functional::get_size_of_array;

} // namespace coin
