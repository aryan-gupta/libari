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

#pragma once

template <typename TType, typename TAlloc>
class DArray {
public:
	class iterator;
	
	using value_type      = TType;
	using size_type       = size_t;
	using reference       = value_type&;
	using const_reference = const value_type&;
	using iterator        = iterator;	

	class iterator {
	public:
		iterator();
		iterator(const TType* data);
		iterator(const iterator& it);
	
		iterator& operator++(); // preincrement
		iterator& operator--();
		
		iterator operator++(int); // postincrement
		iterator operator--(int);
		
		iterator& operator=(const iterator& it);

		iterator& operator-(int scale);
		iterator& operator+(int scale);
		
		bool operator==(const iterator& it);
		
		TType& operator*();
		TType& operator->();
		
		TType* raw();
		
	private:
		TType* mData;
		
	};
	
	DArray();
	DArray(const DArray& other);
	
	DArray(const size_type size, const TType& val = TType());
	
	template <typename ITER>
	DArray(ITER&& begin, ITER&& end);
	
	iterator begin() const;
	iterator end() const;
	
	void clear();
	
	void push_back(const TType& val);
	void pop_back();
	
	void insert(size_type idx, const TType& val);
	void insert(const DArray<TType>::iterator it, const TType& val);
	
	size_type size() const;
	
	const TType& operator[](size_type idx) const;
	TType& operator[](size_type idx);
	
	TType& at(const size_type idx);
	const at(const size_type idx) const;
	
	void erase(DArray<TYPE>::iterator& it);
	void erase(const DArray<TYPE>::iterator& begin, const DArray<TYPE>::iterator& end);
	
private:
	void check_and_reserve();
	
	static const double GROWTH_FACTOR = 2;
	static const size_type INITIAL_CAP   = 5;

	TType* mArray;
	size_type mSize;
	size_type mCap;
	
};


template<typename TYPE>
DArray<TYPE>::DArray() {
	mArray = new TYPE[INITIAL_CAP]; /// @todo research optimal sizes;
	mSize = 0;
	mCap = INITIAL_CAP;
}


template <typename TType>
DArray<TType>::DArray(const DArray& other) {
	mCap = mSize * GROWTH_FACTOR;
	mSize = other.size();
	mArray = new TType[mCap];
	
	for(size_type i = 0; i < mSize; ++i)
		mArray[i] = other.mArray[i];
}


template <typename TType>
DArray<TType>::DArray(const size_type size, const TType& val) {
	mCap = size * GROWTH_FACTOR;
	mSize = size;
	mArray = new TType[mCap];
	
	std::fill(mArray, mArray + mSize, val);
}


template <typename TType>
template <typename ITER>
DArray<TType>::DArray(ITER&& begin, ITER&& end) { /// @todo const end param
	mSize = std::distance(begin, end);
	mCap = mSize * GROWTH_FACTOR;
	mArray = new TType[mCap];
	
	for(size_type i = 0; i < mSize; ++i)
		mArray[i] = *begin++;
}


template <typename TType>
typename DArray<TType>::iterator DArray<TType>::begin() const {
	/// @todo This part after we define DArray<TType>::iterator
}

template <typename TType>
typename DArray<TType>::iterator DArray<TType>::end() const {
	/// @todo This part after we define DArray<TType>::iterator
}

template <typename TType>
void DArray<TType>::clear() {
	delete mArray;
	
	mSize = 0;
	mCap = INITIAL_CAP;
	mArray = new TType[mCap];
}


template <typename TType>
void DArray<TType>::push_back(const TType& val) {	
	mArray[mSize++] = val;
}


template <typename TType>
void DArray<TType>::pop_back() {
	/// @todo Maybe make it shrink if mSize is less than mCap/2 ?
	mSize--;
}


template <typename TType>
void DArray<TType>::insert(size_type idx, const TType& val) {
	if(mSize == mCap) {
		mCap = mSize * 2;
		tmpArray = new TType[mCap];
		
		size_type last = mSize++;
		while(last --> idx)
			tmpArray[last + 1] = mArray[last];
		
		while(last --> 0)
			tmpArray[last] = mArray[last];
		
		delete mArray;
		mArray = tmpArray;
	} else {
		size_type last = mSize++;
		while(last --> idx)
			mArray[last + 1] = mArray[last];
	}
	
	mArray[idx] = val;
}


template <typename TType>
void DArray<TType>::insert(const DArray<TType>::iterator it, const TType& valz) {
	
}


template <typename TType>
size_type DArray<TType>::size() {
	return mSize;
}


template <typename TType>
const TType& DArray<TType>::operator[](size_type idx) const {
	return mArray[idx];
}


template <typename TType>
TType& DArray<TType>::operator[](size_type idx) {
	return mArray[idx];
}

template <typename TType>
TType& DArray<TType>::at(const size_type idx) {
	if(idx >= mSize) {
		throw std::out_of_range("The specified index: " + std::to_string(idx) + ", is out of range");
	}
	
	return mArray[idx];
}

template <typename TType>
const TType& DArray<TType>::at(const size_type idx) {
	if(idx >= mSize) {
		throw std::out_of_range("The specified index: " + std::to_string(idx) + ", is out of range");
	}
	
	return mArray[idx];
}


template <typename TType>
const TType& DArray<TType>::check_and_reserve() {
	if(mSize == mCap) {
		mCap = mSize * 2;
		tmpArray = new TType[mCap];
		
		for(size_type i = 0; i < mSize; ++i)
			tmpArray[i] = mArray[i];
		
		delete mArray;
		mArray = tmpArray;
	}
}

template <typename TType>
void DArray<TType>::erase(DArray<TType>::iterator it) {
	// go from the start to the end 
	// and move everything back
	while(++it != end) {
		*(it - 1) = *it; 
	}
	
	mSize--;
}

template <typename TType>
DArray<TType>::iterator::iterator() {
	mData = nullptr;
}


template <typename TType>
DArray<TType>::iterator::iterator(const TType* data) {
	mData = data;
}


template <typename TType>
DArray<TType>::iterator::iterator(const iterator& it) {
	mData = it.mData;
}

template <typename TYPE>
typename DArray<TYPE>::iterator& DArray<TYPE>::iterator::operator++() {
	mData++;
	
	return *this;
}

template <typename TYPE>
typename DArray<TYPE>::iterator& DArray<TYPE>::iterator::operator--() {
	mData--;
	
	return *this 
}

template <typename TYPE>
typename DArray<TYPE>::iterator DArray<TYPE>::iterator::operator++(int) {
	return iterator(mData++);
}

template <typename TYPE>
typename DArray<TYPE>::iterator DArray<TYPE>::iterator::operator--(int) {
	return iterator(mData--);
}

template <typename TYPE>
typename DArray<TYPE>::iterator& DArray<TYPE>::iterator::operator=(const iterator& it) {
	data = it.data;
}

template <typename TYPE>
typename DArray<TYPE>::iterator& DArray<TYPE>::iterator::operator+(int scale) {
	mData += scale
	return *this;
}

template <typename TYPE>
typename DArray<TYPE>::iterator& DArray<TYPE>::iterator::operator-(int scale) {
	mData -= scale
	return *this;
}

template <typename TType>
bool DArray<TType>::iterator::operator==(const iterator& it) {
	return mData == it.mData;
}

template <typename TType>
TType& DArray<TType>::iterator::operator*() {
	return *mData;
}

template <typename TType>
TType& DArray<TType>::iterator::operator->() {
	return *mData;
}

template <typename TType>
TType* DArray<TType>::iterator::raw() {
	return mData;
}