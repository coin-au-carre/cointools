#pragma once 

#include <random>
#include <algorithm>
#include <limits>

namespace coin {

namespace _impl_random {

template <typename T>
using uniform_distribution = std::conditional_t<
	std::is_floating_point<T>::value,
	std::uniform_real_distribution<T>,
	std::uniform_int_distribution<T>
>;

template<typename TContainer>
void uniform_randomizer(
	TContainer& container, 
	const typename TContainer::value_type min =  0, 
	const typename TContainer::value_type max =  1) {

 	using std::begin; using std::end;

	std::random_device rd;
	std::mt19937 gen{rd()};

	uniform_distribution<typename TContainer::value_type> distribution(min, max);
 
	auto lambda_norm_dist = [&](){ return distribution(gen); };
	std::generate(begin(container), end(container), lambda_norm_dist);
}

} // namesdpace _impl_random


using _impl_random::uniform_randomizer;

} // namespace coin
