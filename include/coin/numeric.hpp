#pragma once

#include <numeric>

namespace coin {

namespace _impl_numeric {

// also called scalar product or inner product
template<class Vector>
typename Vector::value dot_product(const Vector& u, const Vector& v) {
	using std::begin; using std::end;
	return std::inner_product(begin(u), end(u), begin(v), 0.0);
}

// template<size_t N,class Vector>
// constexpr Vector meta_dot_product(const Vector& u, const Vector& v) {
// 	return N > 0 ? u[N] * v[N] + meta_dot_product<N-1,Vector>(u,v) : meta_dot_product<0,Vector>(u,v);
// }

template <size_t N, class T> 
struct DotProduct  { 
	static T eval(const T* a, const T* b) { 
		return  DotProduct<1,T>::eval(a,b)  + DotProduct<N-1,T>::eval(a + 1,b + 1);  
	} 
};

template <class T> 
struct DotProduct<1,T> { 
	static T eval(const T* a, const T* b) { 
		return (*a)*(*b); 
	}
};

template <size_t N, class T> 
inline T meta_dot_product(const T* a, const T* b)  { return DotProduct<N, T>::eval(a, b); }


template <size_t N, class T> 
inline T meta_dot_product(const std::array<T,N>& a, const std::array<T,N>& b)  { return DotProduct<N, T>::eval(a.data(), b.data()); }


} // namespace _impl_numeric

using _impl_numeric::dot_product;
using _impl_numeric::meta_dot_product;

} // namespace coin
