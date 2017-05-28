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

template <typename TYPE>
class DArray {
public:
	class iterator {
	public:
		iterator();
		iterator(const Node* node);
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
		TYPE* data;
		
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
	mArray = new TYPE[other.size() * 2];
	mSize = other.size();
	mCap = mSize * 2;
	
	for(size_t i = 0; i < mSize; ++i)
		mArray[i] = other.mArray[i];
}


