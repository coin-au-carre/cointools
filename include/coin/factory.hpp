#pragma once 

#include <memory>
#include <vector>
#include <iterator> 

namespace coin {

namespace _impl_functional {

template <class T>
auto make_shared_vector(std::vector<T>& v) {
	std::vector<std::shared_ptr<T>> v_shared_ptr;
	v_shared_ptr.reserve(v.size());
	for (auto && el : v) { 
		v_shared_ptr.emplace_back(std::make_shared<T>(el));
	}
	return v_shared_ptr;
}

template <class T>
auto make_unique_vector(std::vector<T>& v) {
	std::vector<std::unique_ptr<T>> v_smart_ptr;
	v_smart_ptr.reserve(v.size());
	for (auto &&el : v) { 
		v_smart_ptr.emplace_back(std::make_unique<T>(el));
	}
	return v_smart_ptr;
}


} // namespace _impl_functional

using _impl_functional::make_shared_vector;
using _impl_functional::make_unique_vector;

} // namespace coin
