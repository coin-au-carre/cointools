#pragma once

#include <memory>
#include <utility>

namespace coin {

template<typename T>
class pimpl 
{
public:
    pimpl() : ptr_{new T{}} {}
    template<typename ...Args> pimpl(Args&& ... args) : ptr_{ new T{ std::forward<Args>(args)... } } { }
    ~pimpl() {}
    
    T* operator->() const { return ptr_.get();  }
    T& operator*()  const { return *ptr_.get(); }
    
private:
    std::unique_ptr<T> ptr_;
};


template <class T>
class pimpl_ptr_deep
{
public:	
	pimpl_ptr_deep() : ptr_{new T{}} {}
	template<typename ...Args> pimpl_ptr_deep(Args&& ... args) : ptr_{ new T{ std::forward<Args>(args)... } } { }
	pimpl_ptr_deep(T *) = delete;
	pimpl_ptr_deep(const pimpl_ptr_deep &src) : ptr_(new T(*src.ptr_)) {}
	// pimpl_ptr_deep(pimpl_ptr_deep &&) = default;
	pimpl_ptr_deep& operator=(const pimpl_ptr_deep &src) { ptr_.reset(new T(*src.ptr_)); return *this; }
	// pimpl_ptr_deep& operator=(pimpl_ptr_deep &&) = default;
	~pimpl_ptr_deep() = default;
	
	T* operator->() { return ptr_.get();  }
	T& operator*() { return *ptr_.get(); }
	
private:
	std::unique_ptr<T> ptr_;
};


} // namespace coin
