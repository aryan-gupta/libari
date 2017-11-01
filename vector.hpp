/* 
 * Copyright (c) 2017 The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>
 * 
 * =============================================================================
 * @author 			Aryan Gupta
 * @project 		
 * @title 			
 * @date			(YYYY-MM-DD)
 * @fversion		1.0.0
 * @description 	
 * =============================================================================
 */

#ifndef ARI_VECTOR_HPP
#define ARI_VECTOR_HPP

#include <memory>
#include <limits>

#include "Iterator.h"

namespace ari {

template <typename TType, typename TAlloc = std::allocator<TType>>
class vector {
public:	
	using value_type      = TType;
	using allocator_type  = TAlloc;
	using size_type       = size_t;
	using reference       = value_type&;
	using const_reference = const value_type&;	
	using pointer         = typename std::allocator_traits<allocator_type>::pointer;
	using const_pointer   = typename std::allocator_traits<allocator_type>::const_pointer;
	using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
	
	using iterator               = ari::random_access_iterator<pointer>;
	using const_iterator         = ari::random_access_iterator<const_pointer>;
	using reverse_iterator       = ari::reverse_iterator<pointer>;
	using const_reverse_iterator = ari::reverse_iterator<const_pointer>;
	
	
	vector();
	explicit vector(const allocator_type& alloc);
	vector(const size_type size, const_reference val = value_type{}, const allocator_type& alloc = allocator_type{});
	vector(const vector& other);
	vector(vector&& other);
	vector(vector&& other, const allocator_type& alloc);
	vector(std::initializer_list<value_type> init);
	template <typename TIter> vector(TIter&& begin, TIter&& end);
	
	~vector();
	
	void assign(size_type count, const_reference val);
	template <typename TIter> void assign(const TIter& begin, const TIter& end);
	void assign(std::initializer_list<value_type>& init);
	
	iterator begin() const;
	iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;
	reverse_iterator rbegin() const;
	reverse_iterator rend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;
	
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;
	
	pointer data();
	const_pointer data() const;
	
	bool empty() const;
	size_type size() const;
	size_type max_size() const;
	void reserve(size_type sz);
	void reserve(size_type sz, const_reference value);
	size_type capacity() const;
	void shrink_to_fit();
	void clear();
	
	void push_back(const_reference val);
	void push_back(value_type&& val);
	void pop_back();

	void insert(size_type idx, const_reference val); // maybe should remove this soon
	iterator insert(const_iterator it, const_reference val);
	reverse_iterator insert(const_reverse_iterator it, const_reference val);
	iterator insert(const_iterator it, value_type&& val);
	reverse_iterator insert(const_reverse_iterator it, value_type&& val);
	iterator insert(const_iterator it, size_type num, const_reference val);
	reverse_iterator insert(const_reverse_iterator it, size_type num, const_reference val);
	template <typename TIter> iterator insert(const_iterator it, const TIter& begin, const TIter& end);
	template <typename TIter> reverse_iterator insert(const_reverse_iterator it, const TIter& begin, const TIter& end);
	iterator insert(const_iterator it, std::initializer_list<value_type> lst);
	reverse_iterator insert(const_reverse_iterator it, std::initializer_list<value_type> lst);
	
	template <typename... TArgs> iterator emplace(const_iterator it, TArgs&&... args);
	template <typename... TArgs> reverse_iterator emplace(const_reverse_iterator it, TArgs&&... args);
	template <typename... TArgs> reference emplace_back(TArgs&&... args);
	
	const_reference operator[](const size_type idx) const;
	reference operator[](const size_type idx);
	
	const_reference at(const size_type idx) const;
	reference at(const size_type idx);
	
	iterator erase(const_iterator it);
	reverse_iterator erase(const_reverse_iterator it);
	iterator erase(const_iterator begin, const_iterator end);
	reverse_iterator erase(const_reverse_iterator begin, const_reverse_iterator end);
	
private:	
	[[deprecated]] void expand_to(size_type sz); // depreciated
	/// Moves [idx, mSize] forward by one, leaving a gap at idx and increasing mSize by one
	pointer move_up(pointer pos, size_type num = 1);
	
	pointer insert_base(pointer pos, const_reference val, size_type num = 1);
	pointer insert_base(pointer pos, value_type&& val);
	template <typename TIter> pointer insert_base(pointer pos, TIter begin, TIter end);
	
	pointer erase_base(pointer pos);
	pointer erase_base(pointer begin, pointer end);
	
	static constexpr double GROWTH_FACTOR  = 2;
	static constexpr size_type INITIAL_CAP = 5;

	size_type mSize;
	size_type mCap;
	allocator_type mAlloc;
	pointer mArray;
	
};


template<typename TType, typename TAlloc>
vector<TType, TAlloc>::vector() 
: mSize{0}, mCap{INITIAL_CAP}, mAlloc{}, mArray{mAlloc.allocate(mCap)} { /* No Code */ }


template<typename TType, typename TAlloc>
vector<TType, TAlloc>::vector(const allocator_type& alloc) 
: mSize{0}, mCap{INITIAL_CAP}, mAlloc{alloc}, mArray{mAlloc.allocate(mCap)} { /* No Code */ }


template<typename TType, typename TAlloc>
vector<TType, TAlloc>::vector(const size_type size, const_reference val, const allocator_type& alloc)
: mSize{size}, mCap{mSize * GROWTH_FACTOR}, mAlloc{alloc}, mArray{mAlloc.allocate(mCap)}
	{ std::fill(mArray, mArray + mSize, val); }


template <typename TType, typename TAlloc>
vector<TType, TAlloc>::vector(const vector& other)
: mSize{other.mSize}, mCap{other.mCap}, mAlloc{other.mAlloc}, mArray{mAlloc.allocate(mCap)}
	{ std::copy(other.begin(), other.end(), mArray); }


template <typename TType, typename TAlloc>
vector<TType, TAlloc>::vector(vector&& other)
: mSize{other.mSize}, mCap{other.mCap}, mAlloc{other.mAlloc}, mArray{other.mArray} {
	// need to do more reaserch on whether allocators can be moved
	other.mArray = pointer{}; // or should I change it to allocate INITIAL_CAP?
	other.mSize = size_type{};
	other.mCap = INITIAL_CAP;
}


template <typename TType, typename TAlloc>
vector<TType, TAlloc>::vector(vector&& other, const allocator_type& alloc)
: mSize{other.mSize}, mCap{other.mCap}, mAlloc{alloc}, mArray{other.mArray} {
	// need to do more reaserch on whether allocators can be moved
	other.mArray = pointer{}; // or should I change it to allocate INITIAL_CAP?
	other.mSize = size_type{};
	other.mCap = INITIAL_CAP;
}


template <typename TType, typename TAlloc>
vector<TType, TAlloc>::vector(std::initializer_list<value_type> init)
: mSize{init.size()}, mCap{mSize * GROWTH_FACTOR}, mAlloc{}, mArray{mAlloc.allocate(mCap)}
	{ std::move(init.begin(), init.end(), mArray); }


template <typename TType, typename TAlloc>
template <typename TIter>
vector<TType, TAlloc>::vector(TIter&& begin, TIter&& end)
: mSize{static_cast<size_type>(std::distance(begin, end))},
  mCap{static_cast<size_type>(mSize * GROWTH_FACTOR)}, mAlloc{}, mArray{mAlloc.allocate(mCap)}
	{ std::copy(begin, end, mArray); }


template <typename TType, typename TAlloc>
vector<TType, TAlloc>::~vector()
	{ mAlloc.deallocate(mArray, mCap); }

	
template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::assign(size_type count, const_reference val) {
	mAlloc.deallocate(mArray, mCap);
	
	mSize  = count;
	mCap   = mSize * GROWTH_FACTOR;
	mArray = mAlloc.allocate(mCap);
	
	std::fill(mArray, mArray + mSize, val);
}


template <typename TType, typename TAlloc>
template <typename TIter> 
void vector<TType, TAlloc>::assign(const TIter& begin, const TIter& end) {
	mAlloc.deallocate(mArray, mCap);
	
	mSize  = std::distance(begin, end);
	mCap   = mSize * GROWTH_FACTOR;
	mArray = mAlloc.allocate(mCap);
	
	std::copy(begin, end, mArray);
}


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::assign(std::initializer_list<TType>& init)
	{ assign(init.begin(), init.end()); }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::begin() const -> iterator
	{ return iterator{mArray}; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::end() const -> iterator
	{ return iterator{mArray + mSize}; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::cbegin() const -> const_iterator
	{ return const_iterator{mArray}; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::cend() const -> const_iterator
	{ return const_iterator{mArray + mSize}; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::rbegin() const -> reverse_iterator
	{ return reverse_iterator{mArray + mSize}; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::rend() const -> reverse_iterator
	{ return reverse_iterator{mArray}; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::crbegin() const -> const_reverse_iterator
	{ return const_reverse_iterator{mArray + mSize}; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::crend() const -> const_reverse_iterator
	{ return const_reverse_iterator{mArray}; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::front() -> reference
	{ return *mArray; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::front() const -> const_reference
	{ return *mArray; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::back() -> reference
	{ return *(mArray + mSize - 1); }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::back() const -> const_reference
	{ return *(mArray + mSize - 1); }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::data() -> pointer
	{ return mArray; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::data() const -> const_pointer
	{ return mArray; }


template <typename TType, typename TAlloc>
bool vector<TType, TAlloc>::empty() const
	{ return mSize == 0; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::size() const -> size_type
	{ return mSize; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::max_size() const -> size_type {
	return std::numeric_limits<size_type>::max();
}


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::reserve(size_type sz) {
	if (mSize >= sz) // just a pre check
		return;
	
	pointer tmpArray = mAlloc.allocate(sz);
	std::move(mArray, mArray + mSize, tmpArray);
	mAlloc.deallocate(mArray, mCap);
	mArray = tmpArray;
	mCap = sz;
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::capacity() const -> size_type
	{ return mCap; }


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::shrink_to_fit() {
	pointer tmpArray = mAlloc.allocate(mSize);
	std::move(mArray, mArray + mSize, tmpArray);
	mAlloc.deallocate(mArray, mCap);
	mArray = tmpArray;
	mCap = mSize;
}


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::clear() {
	mAlloc.deallocate(mArray, mCap);
	
	mSize = 0;
	mCap = INITIAL_CAP;
	mArray = mAlloc.allocate(mCap);
}


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::push_back(const_reference val)
	{ this->insert_base(mArray + mSize, val); }


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::push_back(value_type&& val)
	{ this->insert_base(mArray + mSize, std::move(val)); }


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::pop_back() 
	{ mSize--; }


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::insert(size_type idx, const_reference val) {
	this->insert_base(mArray + idx, val);
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::insert(const_iterator it, const_reference val) -> iterator {
	difference_type idx = it - cbegin();
	pointer pos = this->insert_base(mArray + idx, val);
	return iterator{pos};
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::insert(const_reverse_iterator it, const_reference val) -> reverse_iterator {
	difference_type idx = it - crbegin();
	iterator pos = this->insert_base(mArray + idx, val);
	return reverse_iterator{pos};
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::insert(const_iterator it, value_type&& val) -> iterator {
	difference_type idx = it - cbegin();
	pointer pos = this->insert_base(mArray + idx, std::move(val));
	return iterator{pos};
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::insert(const_reverse_iterator it, value_type&& val) -> reverse_iterator {
	difference_type idx = it - crbegin();
	pointer pos = this->insert_base(mArray + idx, std::move(val));
	return reverse_iterator{pos};
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::insert(const_iterator it, size_type num, const_reference val) -> iterator {
	difference_type idx = it - cbegin();
	pointer pos = this->insert_base(mArray + idx, val, num);
	return iterator{pos};
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::insert(const_reverse_iterator it, size_type num, const_reference val) -> reverse_iterator {
	difference_type idx = it - crbegin();
	pointer pos = this->insert_base(mArray + idx, val, num);
	return reverse_iterator{it.base()};
}


template <typename TType, typename TAlloc>
template <typename TIter>
auto vector<TType, TAlloc>::insert(const_iterator it, const TIter& begin, const TIter& end) -> iterator {
	difference_type idx = it - cbegin();
	pointer pos = this->insert_base(mArray + idx, begin, end);
	return iterator{pos};
}


template <typename TType, typename TAlloc>
template <typename TIter>
auto vector<TType, TAlloc>::insert(const_reverse_iterator it, const TIter& begin, const TIter& end) -> reverse_iterator {
	difference_type idx = it - crbegin();
	pointer pos = this->insert_base(mArray + idx, begin, end); // this needs more work. Maybe convert these into reverse iterators?
	// maybe force the user to use reverse iterators and do no work here?
	return reverse_iterator{pos};
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::insert(const_iterator it, std::initializer_list<value_type> lst) -> iterator {
	difference_type idx = it - cbegin();
	pointer pos = this->insert_base(mArray + idx, lst.begin(), lst.end());
	return iterator{pos};
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::insert(const_reverse_iterator it, std::initializer_list<value_type> lst) -> reverse_iterator {
	difference_type idx = it - crbegin();
	pointer pos = this->insert_base(mArray + idx, lst.rbegin(), lst.rend());
	return reverse_iterator{pos};
}


template <typename TType, typename TAlloc>
template <typename... TArgs>
auto vector<TType, TAlloc>::emplace(const_iterator it, TArgs&&... args) -> iterator {
	move_up(it.base() - mArray); /// @todo fix this
	*it = value_type{std::forward<TArgs>(args)...};
}


template <typename TType, typename TAlloc>
template <typename... TArgs>
auto vector<TType, TAlloc>::emplace_back(TArgs&&... args) -> reference {
	return mArray[mSize++] = value_type{std::forward<TArgs>(args)...};
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::operator[](size_type idx) const -> const_reference
	{ return mArray[idx]; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::operator[](size_type idx) -> reference
	{ return mArray[idx]; }


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::at(const size_type idx) const -> const_reference {
	if(idx >= mSize) {
		throw std::out_of_range("The specified index: " + std::to_string(idx) + ", is out of range");
	}
	
	return mArray[idx];
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::at(const size_type idx) -> reference {
	if(idx >= mSize) {
		throw std::out_of_range("The specified index: " + std::to_string(idx) + ", is out of range");
	}
	
	return mArray[idx];
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::erase(const_iterator it) -> iterator {
	this->erase_base(it.base());
	return iterator{it.base()};
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::erase(const_reverse_iterator it) -> reverse_iterator {
	this->erase_base(it.base());
	return reverse_iterator{it.base()};
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::erase(const_iterator begin, const_iterator end) -> iterator {
	this->erase_base(begin.base(), end.base());
	return iterator{begin.base()};
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::erase(const_reverse_iterator begin, const_reverse_iterator end) -> reverse_iterator {
	this->erase_base(end.base(), begin.base());
	return iterator{begin.base()};
}


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::expand_to(size_type sz) { // depreciated
	pointer tmpArray = mAlloc.allocate(sz);
	
	// if the first expression of the ?: operator runs then we are expanding the array
	// if second expression runs then we are contracting the array. In the second case
	// data WILL be lost. 
	std::move(
		mArray, 
		(sz > mSize ? mArray + mSize : mArray + sz), // we want to copy the smaller value
		tmpArray
	);
	
	mAlloc.deallocate(mArray, mCap);
	
	mArray = tmpArray;
	mCap = sz;
}


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::move_up(size_type idx) {
	if(mSize == mCap) {
		size_type newCap = mSize * GROWTH_FACTOR;
		pointer newArray = mAlloc.allocate(newCap);
		
		std::move(mArray, mArray + idx, newArray);
		std::move(mArray + idx, mArray + mSize, newArray + idx + 1);
		
		mAlloc.deallocate(mArray, mCap);
		mArray = newArray;
		mCap = newCap;
		++mSize;
	} else {
		std::move_backward(mArray + idx, mArray + mSize, mArray + mSize + 1);
		++mSize;
	}
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::insert_base(pointer pos, const_reference val, size_type num) -> pointer {
	pos = move_up(pos, num);
	
	// pointer ret = pos;
	
	while (num --> 0)
		*pos++ = val;
	
	// return ret;
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::insert_base(pointer pos, value_type&& val) -> pointer {
	pos = move_up(pos);
	*pos = std::move(val);
	// return pos;
}


template <typename TType, typename TAlloc>
template <typename TIter>
auto vector<TType, TAlloc>::insert_base(pointer pos, TIter begin, TIter end) -> pointer {
	move_up(pos, std::distance(begin, end));
	
	while (begin != end)
		*pos++ = *begin++;
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::erase_base(pointer pos) -> pointer {
	
}


template <typename TType, typename TAlloc>
auto vector<TType, TAlloc>::erase_base(pointer begin, const pointer end) -> pointer {
	
}


} // end namespace ari

#endif // ARI_VECTOR_HPP defined