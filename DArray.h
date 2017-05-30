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

static const double GROWTH_FACTOR = 2;

template <typename TYPE>
class DArray {
public:
	class iterator {
	public:
		iterator();
		iterator(const TYPE* data);
		iterator(const iterator& it);
	
		iterator& operator++(); // preincrement
		iterator& operator--();
		
		iterator operator++(int); // postincrement
		iterator operator--(int);
		
		iterator& operator=(const iterator& it);

		iterator& operator-(int scale);
		iterator& operator+(int scale);
		
		bool operator==(const iterator& it);
		
		TYPE& operator*();
		TYPE& operator->();
		
		TYPE* raw();
		
	private:
		TYPE* mData;
		
	};
	
	DArray();
	DArray(const DArray& other);
	
	DArray(const size_t size, const TYPE& val = TYPE());
	
	template <typename ITER>
	DArray(ITER&& begin, ITER&& end);
	
	iterator begin() const;
	iterator end() const;
	
	void clear();
	
	void push_back(const TYPE& val);
	void pop_back();
	
	void insert(size_t idx, const TYPE& val);
	void insert(const DList<TYPE>::iterator it, const TYPE& val);
	
	size_t size() const;
	
	const TYPE& operator[](size_t idx) const;
	TYPE& operator[](size_t idx);
	
private:
	TYPE* mArray;
	size_t mSize;
	size_t mCap;
	
};


template<typename TYPE>
DArray<TYPE>::DArray() {
	mArray = new TYPE[5]; /// @todo research optimal sizes;
	mSize = 0;
	mCap = 5;
}


template <typename TYPE>
DArray<TYPE>::DArray(const DArray& other) {
	mCap = mSize * GROWTH_FACTOR;
	mSize = other.size();
	mArray = new TYPE[mCap];
	
	for(size_t i = 0; i < mSize; ++i)
		mArray[i] = other.mArray[i];
}


template <typename TYPE>
DArray<TYPE>::DArray(const size_t size, const TYPE& val) {
	mCap = size * GROWTH_FACTOR;
	mSize = size;
	mArray = new TYPE[mCap];
	
	for(size_t i = 0; i < mSize; ++i)
		mArray[i] = val;
}


template <typename TYPE>
template <typename ITER>
DArray<TYPE>::DArray(ITER&& begin, ITER&& end) { /// @todo const end param
	mSize = std::distance(begin, end);
	mCap = mSize * GROWTH_FACTOR;
	mArray = new TYPE[mCap];
	
	for(size_t i = 0; i < mSize; ++i)
		mArray[i] = *begin++;
}


template <typename TYPE>
typename DArray<TYPE>::iterator DArray<TYPE>::begin() const {
	/// @todo This part after we define DArray<TYPE>::iterator
}

template <typename TYPE>
typename DArray<TYPE>::iterator DArray<TYPE>::end() const {
	/// @todo This part after we define DArray<TYPE>::iterator
}

template <typename TYPE>
void DArray<TYPE>::clear() {
	delete mArray;
	
	mSize = 0;
	mCap = 5;
	mArray = new TYPE[mCap];
}


template <typename TYPE>
void DArray<TYPE>::push_back(const TYPE& val) {
	if(mSize == mCap) {
		mCap = mSize * 2;
		tmpArray = new TYPE[mCap];
		
		for(size_t i = 0; i < mSize; ++i)
			tmpArray[i] = mArray[i];
		
		delete mArray;
		mArray = tmpArray;
	}
	
	mArray[mSize++] = val;
}


template <typename TYPE>
void DArray<TYPE>::pop_back() {
	/// @todo Maybe make it shrink if mSize is less than mCap/2 ?
	mSize--;
}


template <typename TYPE>
void DArray<TYPE>::insert(size_t idx, const TYPE& val) {
	if(mSize == mCap) {
		mCap = mSize * 2;
		tmpArray = new TYPE[mCap];
		
		size_t last = mSize++;
		while(last --> idx)
			tmpArray[last + 1] = mArray[last];
		
		while(last --> 0)
			tmpArray[last] = mArray[last];
		
		delete mArray;
		mArray = tmpArray;
	} else {
		size_t last = mSize++;
		while(last --> idx)
			mArray[last + 1] = mArray[last];
	}
	
	mArray[idx] = val;
}


template <typename TYPE>
void DArray<TYPE>::insert(const DList<TYPE>::iterator it, const TYPE& valz) {
	
}


template <typename TYPE>
size_t DArray<TYPE>::size() {
	return mSize;
}


template <typename TYPE>
const TYPE& DArray<TYPE>::operator[](size_t idx) const {
	return mArray[idx];
}


template <typename TYPE>
TYPE& DArray<TYPE>::operator[](size_t idx) {
	return mArray[idx];
}


template <typename TYPE>
DArray<TYPE>::iterator::iterator() {
	mData = nullptr;
}


template <typename TYPE>
DArray<TYPE>::iterator::iterator(const TYPE* data) {
	mData = data;
}


template <typename TYPE>
DArray<TYPE>::iterator::iterator(const iterator& it) {
	mData = it.mData;
}

template <typename TYPE>
typename DArray<TYPE>::iterator& DArray<TYPE>::operator++() {
	mData++;
	
	return *this;
}

template <typename TYPE>
typename DArray<TYPE>::iterator& DArray<TYPE>::operator--() {
	mData--;
	
	return *this 
}

template <typename TYPE>
typename DArray<TYPE>::iterator DArray<TYPE>::operator++(int) {
	return iterator(mData++);
}

template <typename TYPE>
typename DArray<TYPE>::iterator DArray<TYPE>::operator--(int) {
	return iterator(mData--);
}

template <typename TYPE>
typename DArray<TYPE>::iterator& DArray<TYPE>::operator=(const iterator& it) {
	data = it.data;
}

template <typename TYPE>
typename DArray<TYPE>::iterator& DArray<TYPE>::operator+(int scale) {
	mData += scale
	return *this;
}

template <typename TYPE>
typename DArray<TYPE>::iterator& DArray<TYPE>::operator-(int scale) {
	mData -= scale
	return *this;
}

template <typename TYPE>
bool DArray<TYPE>::iterator::operator==(const iterator& it) {
	return mData == it.mData;
}


