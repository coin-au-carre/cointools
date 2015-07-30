#pragma once

#include <array>
#include <vector>
#include <string>
#include <new>
#include <algorithm> // std::copy
#include <stdexcept>

namespace coin { 

namespace _impl_matrix {

// This matrix base class allows to have a generic storage while providing convenient matrix methods

template<typename T, size_t Rows, size_t Cols>
using DefaultStorage = std::conditional_t<Rows == 0 and Cols == 0, std::vector<T>, std::array<T, Cols*Rows>>;

template<typename T, size_t Rows, size_t Cols, 
	typename MatrixDerived, 
	typename Storage = DefaultStorage<T,Rows,Cols>
	>
class MatrixBase {
public:
	using data_storage    = Storage;
	using value_type      = typename data_storage::value_type;     
	using reference       = typename data_storage::reference;     
	using const_reference = typename data_storage::const_reference;
	using size_type       = typename data_storage::size_type;      
	using iterator        = typename data_storage::iterator;       
	using const_iterator  = typename data_storage::const_iterator;
	
	size_type rows() const { return static_cast<const MatrixDerived *>(this)->impl_rows(); };
	size_type cols() const { return static_cast<const MatrixDerived *>(this)->impl_cols(); };
	size_type size() const { return rows() * cols(); }

		  T* data()       { return data_.data(); }
	const T* data() const { return data_.data(); } 

	iterator begin() { return data_.begin(); }
	const_iterator begin() const { return data_.begin(); }
	const_iterator cbegin() const { return data_.cbegin(); }
	iterator end() { return data_.end(); }
	const_iterator end() const { return data_.cend(); }
	const_iterator cend() const { return data_.cend(); }
 
	reference operator[] (size_type index) { return data_[index]; };
	const_reference operator[] (size_type index) const {
		return const_cast<reference>(static_cast<const MatrixBase&>(*this)[index]);
	};
	
	reference operator() (size_type row, size_type col) { return data_[row * cols() + col]; };
	const_reference operator() (size_type row, size_type col) const {
		return const_cast<reference>(static_cast<const MatrixBase&>(*this)(row,col));
	};
	reference operator[] (const std::array<size_type,2>& indices) { 
		return (*this)(indices[0],indices[1]); 
	}
	const_reference operator[] (const std::array<size_type,2>& indices) const { 
		return const_cast<reference>(static_cast<const MatrixBase&>(*this)(indices[0],indices[1])); 
	}

	const_reference at(size_type row, size_type col) const {
		if (row * cols() + col >= size())
			throw std::out_of_range("matrix indices are out of range");
		return (*this)(row,col);
	}
	
	auto to_string() const {
		using std::to_string;
		std::string str = to_string(rows()) + "x" + to_string(cols()) + "\n";
		size_type i{0};
		for (const auto& el : data_) {
			str += to_string(el) + " ";
			if (!(++i % cols())) { str += "\n"; }
		}
		return str;
	}
	
protected:
	MatrixBase() : data_() {}
	MatrixBase(size_type r, size_type c) : data_(r * c) {}
	MatrixBase(const data_storage& d) : data_(d)  {}
	
private:
	data_storage data_;
};


template<typename T, size_t Rows, size_t Cols, class MatrixDerived, typename Storage>
std::ostream &operator<<(std::ostream &os, const MatrixBase<T,Rows,Cols,MatrixDerived,Storage>& that) {
	return os << that.to_string();
}


template<typename T, size_t Rows, size_t Cols = Rows>
class MatrixStack : public MatrixBase<T,Rows,Cols,MatrixStack<T,Rows,Cols>> {
public:
	using size_type = typename MatrixStack::size_type;
	MatrixStack(const typename MatrixStack::data_storage& d) 
		: MatrixBase<T,Rows,Cols,MatrixStack<T,Rows,Cols>>(d) 
		{}
	size_type impl_rows() const { return Rows; }
	size_type impl_cols() const { return Cols; }
};

template<typename T>
class MatrixHeap : public MatrixBase<T,0,0,MatrixHeap<T>> {
public:
	using size_type = typename MatrixHeap::size_type;
	MatrixHeap(size_type r, size_type c) 
		: MatrixBase<T,0,0,MatrixHeap<T>>(r,c)
		, rows_(r)
		, cols_(c) 
		{}
	MatrixHeap(const typename MatrixHeap::data_storage& d, size_type rows, size_type cols) 
		: MatrixBase<T,0,0,MatrixHeap<T>>(d)
		, rows_{rows}
		, cols_{cols}
		{}
	 
	size_type impl_rows() const { return rows_; }
	size_type impl_cols() const { return cols_; }
	
private:
	size_type rows_;
	size_type cols_;
};


template<typename T>
class RawStorage {
public:
	using value_type      = T;     
	using reference       = T&;
	using pointer         = T*;
	using const_reference = const T&;
	using size_type       = size_t;       
	using iterator        = T*;       
	using const_iterator  = const T*;
	
	RawStorage(size_type len) : raw_data_(new (std::nothrow) T[len]), size_(len) {}
	~RawStorage() { delete[] raw_data_; }
	
	RawStorage(const RawStorage& that) {
		raw_data_ = new T[that.size()];
		std::copy(std::begin(that), std::end(that), begin());
	}
	
	size_type size() const { return size_; }
	
		  T* data()       { return raw_data_; }
	const T* data() const { return raw_data_; } 

	iterator       begin()       { return &raw_data_[0]; }
	const_iterator begin() const { return &raw_data_[0]; }
	iterator       end()         { return &raw_data_[size_]; }
	const_iterator end()   const { return &raw_data_[size_]; }
private:
	pointer   raw_data_;
	size_type size_;
};

template<typename T>
class MatrixHeapRaw : public MatrixBase<T,0,0,MatrixHeapRaw<T>,RawStorage<T>> {
public:
	using size_type = typename MatrixHeapRaw::size_type;
	MatrixHeapRaw(size_type r, size_type c) 
		: MatrixBase<T,0,0,MatrixHeapRaw<T>,RawStorage<T>>(r * c)
		, rows_(r)
		, cols_(c) 
		{}
	MatrixHeapRaw(const typename MatrixHeapRaw::data_storage& d, size_type rows, size_type cols) 
		: MatrixBase<T,0,0,MatrixHeapRaw<T>,RawStorage<T>>(d)
		, rows_{rows}
		, cols_{cols}
		{}
	
	size_type impl_rows() const { return rows_; }
	size_type impl_cols() const { return cols_; }
	
private:
	size_type rows_;
	size_type cols_;
};


} // namespace _impl_matrix

using _impl_matrix::MatrixHeap;
using _impl_matrix::MatrixStack;
using _impl_matrix::MatrixHeapRaw;
using _impl_matrix::operator<<;

} // namespace coin

