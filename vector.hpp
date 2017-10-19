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

#ifndef ARI_VECTOR_H
#define ARI_VECTOR_H

#include <memory>

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
	
	using iterator               = ari::random_access_iterator<pointer>;
	using const_iterator         = ari::random_access_iterator<const_pointer>;
	using reverse_iterator       = ari::reverse_iterator<pointer>;
	using const_reverse_iterator = ari::reverse_iterator<const_pointer>;
	
	
	vector();
	vector(const vector<TType, TAlloc>& other);
	
	vector(const size_type size, const_reference val = TType());
	
	template <typename TIter>
	vector(TIter&& begin, TIter&& end);
	
	~vector();
	
	iterator begin() const;
	iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;
	reverse_iterator rbegin() const;
	reverse_iterator rend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;
	
	void clear();
	size_type max_size() const;
	
	void push_back(const_reference val);
	void pop_back();
	
	void insert(size_type idx, const_reference val);
	void insert(const iterator& it, const_reference val);
	
	size_type size() const;
	
	const_reference operator[](const size_type idx) const;
	reference operator[](const size_type idx);
	
	const_reference at(const size_type idx) const;
	reference at(const size_type idx);
	
	void erase(iterator it);
	//void erase(const iterator& begin, const iterator& end);
	
private:	
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


template <typename TType, typename TAlloc>
vector<TType, TAlloc>::vector(const vector& other)
: mSize{other.mSize}, mCap{other.mCap}, mAlloc{other.mAlloc}, mArray{mAlloc.allocate(mCap)} {
	std::copy(other.begin(), other.end(), mArray);
}


template <typename TType, typename TAlloc>
vector<TType, TAlloc>::vector(const size_type size, const TType& val) 
: mSize{size}, mCap{size * GROWTH_FACTOR}, mAlloc{}, mArray{mAlloc.allocate(mCap)} {
	std::fill(mArray, mArray + mSize, val);
}


template <typename TType, typename TAlloc>
template <typename TIter>
vector<TType, TAlloc>::vector(TIter&& begin, TIter&& end)
: mSize{static_cast<size_type>(std::distance(begin, end))}, /// @todo const end param
  mCap{static_cast<size_type>(mSize * GROWTH_FACTOR)}, mAlloc{}, mArray{mAlloc.allocate(mCap)} {
	std::copy(begin, end, mArray);
}


template <typename TType, typename TAlloc>
vector<TType, TAlloc>::~vector() {
	mAlloc.deallocate(mArray, mCap);
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::iterator vector<TType, TAlloc>::begin() const {
	return iterator{mArray};
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::iterator vector<TType, TAlloc>::end() const {
	return iterator{mArray + mSize};
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::const_iterator vector<TType, TAlloc>::cbegin() const {
	return const_iterator{mArray};
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::const_iterator vector<TType, TAlloc>::cend() const {
	return const_iterator{mArray + mSize};
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::reverse_iterator vector<TType, TAlloc>::rbegin() const {
	return reverse_iterator{mArray + mSize};
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::reverse_iterator vector<TType, TAlloc>::rend() const {
	return reverse_iterator{mArray};
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::const_reverse_iterator vector<TType, TAlloc>::crbegin() const {
	return const_reverse_iterator{mArray + mSize};
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::const_reverse_iterator vector<TType, TAlloc>::crend() const {
	return const_reverse_iterator{mArray};
}


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::clear() {
	mAlloc.deallocate(mArray, mCap);
	
	mSize = 0;
	mCap = INITIAL_CAP;
	mArray = mAlloc.allocate(mCap);
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::size_type vector<TType, TAlloc>::max_size() const {
	return mCap;
}


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::push_back(typename vector<TType, TAlloc>::const_reference val) {
	if(mSize == mCap) {
		size_type newCap = mSize * 2;
		pointer newArray = mAlloc.allocate(newCap);
		
		std::copy(mArray, mArray + mSize, newArray);
		
		mAlloc.deallocate(mArray, mCap);
		
		mArray = newArray;
		mCap = newCap;
	}
	
	mArray[mSize++] = val;
}


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::pop_back() {
	mSize--;
}


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::insert(
	typename vector<TType, TAlloc>::size_type idx,
	typename vector<TType, TAlloc>::const_reference val
) {
	if(mSize == mCap) {
		size_type newCap = mSize * 2;
		pointer newArray = mAlloc.allocate(newCap);
		
		size_type last = mSize++;
		while(last --> idx)
			newArray[last + 1] = mArray[last];
		
		while(last --> 0)
			newArray[last] = mArray[last];
		
		mAlloc.deallocate(mArray, mCap);
		mArray = newArray;
		mCap = newCap;
	} else {
		size_type last = mSize++;
		while(last --> idx)
			mArray[last + 1] = mArray[last];
	}
	
	mArray[idx] = val;
}


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::insert(
	const typename vector<TType, TAlloc>::iterator& it,
	typename vector<TType, TAlloc>::const_reference val
) {
	/* Need to Code */
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::size_type vector<TType, TAlloc>::size() const {
	return mSize;
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::const_reference 
vector<TType, TAlloc>::operator[](typename vector<TType, TAlloc>::size_type idx) const {
	return mArray[idx];
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::reference
vector<TType, TAlloc>::operator[](typename vector<TType, TAlloc>::size_type idx) {
	return mArray[idx];
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::const_reference
vector<TType, TAlloc>::at(const typename vector<TType, TAlloc>::size_type idx) const {
	if(idx >= mSize) {
		throw std::out_of_range("The specified index: " + std::to_string(idx) + ", is out of range");
	}
	
	return mArray[idx];
}


template <typename TType, typename TAlloc>
typename vector<TType, TAlloc>::reference
vector<TType, TAlloc>::at(const typename vector<TType, TAlloc>::size_type idx) {
	if(idx >= mSize) {
		throw std::out_of_range("The specified index: " + std::to_string(idx) + ", is out of range");
	}
	
	return mArray[idx];
}


template <typename TType, typename TAlloc>
void vector<TType, TAlloc>::erase(typename vector<TType, TAlloc>::iterator it) {
	// go from the start to the end 
	// and move everything back
	while(++it != end) {
		*(it - 1) = *it; 
	}
	
	mSize--;
}

} // end namespace ari

#endif // ARI_VECTOR_H defined