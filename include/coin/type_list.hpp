#pragma once


namespace coin {

namespace _impl_math {

template<class... T> struct type_list {};


template<class A, template<class...> class B> struct meta_rename_impl;

template<template<class...> class A, class... T, template<class...> class B>
struct meta_rename_impl<A<T...>, B> {
	using type = B<T...>;
};

template<class A, template<class...> class B>
using mp_rename = typename meta_rename_impl<A, B>::type;


} // namespace _impl_math

} // namespace coin

