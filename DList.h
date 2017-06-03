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
class DList {
public:
	struct Node {
		Node* prev;
		TYPE data;
		Node* next;
	};
	
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
		bool operator!=(const iterator& it);
		
		TYPE& operator*();
		TYPE& operator->();
		
		Node* getNode();
		
	private:
		Node* data;
		
	};
	
	DList();
	DList(const DList& other);
	
	DList(const size_t size, const TYPE& val = TYPE());
	
	template <typename ITER, typename = decltype(*ITER())>
	DList(ITER&& begin, ITER&& end);
	
	iterator begin() const;
	iterator end() const;
	
	void clear();
	
	void push_back(const TYPE& val);
	void push_front(const TYPE& val);
	
	void pop_back();
	void pop_front();
	
	void insert(size_t idx, const TYPE& val);
	void insert(const DList<TYPE>::iterator it, const TYPE& val);
	void remove(const TYPE& val);
	
	size_t size() const;
	
	const TYPE& operator[](size_t idx) const;
	TYPE& operator[](size_t idx);
	
private:	
	Node* mHead; /// @todo convert to iterators?
	Node* mTail;
	size_t mSize;
	
};


// IMPLEMENTATION

template <typename TYPE> 
DList<TYPE>::DList() {
	mHead = nullptr;
	mTail = nullptr;
	mSize = 0;
}


template <typename TYPE> 
DList<TYPE>::DList(const DList& other) {
	mHead = nullptr;
	mSize = 0;
	
	if(other.size() > 0) {
		mHead = new Node{nullptr, other[0], nullptr};
		mSize++;
	}
	
	Node* current = mHead, *prev = nullptr;
	
	for(size_t i = 1; i < other.size(); ++i) {
		prev = current;
		current = new Node{prev, other[i], nullptr};
		prev->next = current;
		
		mSize++;
	}
	
	mTail = current;
}


template <typename TYPE> 
DList<TYPE>::DList(const size_t size, const TYPE& val) {
	mHead = nullptr;
	mSize = 0; /// @todo make one statement later rather than incrementing everytime
	
	if(size > 0) {
		mHead = new Node{nullptr, val, nullptr};
		mSize++;
	}
	
	Node* current = mHead, *prev = nullptr;
	
	for(size_t i = 0; i < size; ++i) {
		prev = current;
		current = new Node{prev, val, nullptr};
		prev->next = current;
		
		mSize++;
	}
	
	mTail = current;
}

template <typename TYPE>
template <typename ITER, typename>
DList<TYPE>::DList(ITER&& begin, ITER&& end) {
	mHead = nullptr;
	mSize = 0;
	
	if(std::distance(begin, end) > 0) {
		mHead = new Node{nullptr, *begin++, nullptr};
		mSize++;
	}
	
	Node* current = mHead, *prev = nullptr;
	
	while(begin != end) {
		prev = current;
		current = new Node{prev, *begin++, nullptr};
		prev->next = current;
		
		mSize++;
	}
	
	mTail = current;
}


template <typename TYPE> 
typename DList<TYPE>::iterator DList<TYPE>::begin() const {
	return iterator(mHead);
}


template <typename TYPE> 
typename DList<TYPE>::iterator DList<TYPE>::end() const {
	/// @todo this part
	// maybe add tail member variable so we dont have 
	// to calculate it everytime we call this func?
}


template <typename TYPE> 
void DList<TYPE>::clear() {
	while(mHead != nullptr) {
		Node* rem = mHead;
		mHead = mHead->next;
		delete rem;
	}
	mHead = nullptr;
	mTail = nullptr;
	
	mSize = 0;
}


template <typename TYPE> 
void DList<TYPE>::push_back(const TYPE& val) {
	if(mHead == nullptr && mTail == nullptr)
		mHead = mTail = new Node{nullptr, val, nullptr};
	else
		mTail = mTail->next = new Node{mTail, val, nullptr};
	
	mSize++;
}


template <typename TYPE> 
void DList<TYPE>::push_front(const TYPE& val) {
	if(mHead == nullptr && mTail == nullptr)
		mHead = mTail = new Node{nullptr, val, nullptr};
	else
		mHead = mHead->prev = new Node{nullptr, val, mHead};
	
	mSize++;
}


template <typename TYPE> 
void DList<TYPE>::pop_back() {
	if(mSize == 0)
		throw std::out_of_range("No elements to remove");
	
	Node* rem = mTail;
	mTail = mTail->prev;
	delete rem;
	mTail->next = nullptr;
	
	mSize--;
	if(mSize == 0) /// @todo Check mSize is 1 condition (do we need to change mTail to nullptr?)
		mHead = nullptr;
}


template <typename TYPE> 
void DList<TYPE>::pop_front() {
	if(mSize == 0)
		throw std::out_of_range("No elements to remove");
	
	Node* rem = mHead;
	mHead = mHead->next;
	delete rem;
	mHead->prev = nullptr;
	
	mSize--;
	if(mSize == 0)
		mTail = nullptr;
}


template <typename TYPE> 
void DList<TYPE>::insert(size_t idx, const TYPE& val) {
	if(idx > mSize)
		throw std::out_of_range("Supplied index was out of range");
	
	if       (idx == 0) {
		push_front(val);
	} else if(idx == mSize) {
		push_back(val);
	} else {
		Node* current = mHead;
		
		while(idx --> 0)
			current = current->next;
		
		Node* newPrev = current->prev;
		current->prev = new Node{newPrev, val, current};
		newPrev->next = current->prev;
		
		mSize++;
	}
}


template <typename TYPE> 
void DList<TYPE>::insert(const DList<TYPE>::iterator it, const TYPE& val) {
	Node* current = it.getNode();
	Node* newPrev = current->prev;
	
	current->prev = new Node{newPrev, val, current};
	
	mSize++;
}


template <typename TYPE> 
void DList<TYPE>::remove(const TYPE& val) {
	Node* current = mHead, *rem = nullptr;
	
	while(current != nullptr) {
		if(current->data == val) {
			rem = current; /// @todo we can shorted this by a line
			
			if       (rem == mTail) {
				mTail = mTail->prev;
				mTail->next = nullptr;
			} else if(rem == mHead) {
				mHead = mHead->next;
				mHead->prev = nullptr;
			} else {
				current->prev->next = current->next;
				current->next->prev = current->prev;
			}
			
			current = current->next;
			delete rem;
			
			mSize--;
		} else {
			current = current->next;
		}
	}
}


template <typename TYPE> 
size_t DList<TYPE>::size() const {
	return mSize;
}


template <typename TYPE> 
const TYPE& DList<TYPE>::operator[](size_t idx) const {
	Node* current = mHead;
	
	while(idx --> 0)
		current = current->next;
	
	return current->data;
}


template <typename TYPE> 
TYPE& DList<TYPE>::operator[](size_t idx) {
	Node* current = mHead;
	
	while(idx --> 0)
		current = current->next;
	
	return current->data;
}

// ITERATOR


template <typename TYPE> 
DList<TYPE>::iterator::iterator() {
	data = nullptr;
}


template <typename TYPE> 
DList<TYPE>::iterator::iterator(const Node* node) {
	data = node;
}


template <typename TYPE> 
DList<TYPE>::iterator::iterator(const DList<TYPE>::iterator& it) {
	data = it.data;
}


template <typename TYPE> 
typename DList<TYPE>::iterator& DList<TYPE>::iterator::operator++() {
	data = data->next;
	
	return *this;
}


template <typename TYPE> 
typename DList<TYPE>::iterator& DList<TYPE>::iterator::operator--() {
	data = data->prev;
	
	return *this;
}


template <typename TYPE> 
typename DList<TYPE>::iterator DList<TYPE>::iterator::operator++(int) {
	DList<TYPE>::iterator orig(data);
	
	data = data->next;
	
	return orig;
}


template <typename TYPE> 
typename DList<TYPE>::iterator DList<TYPE>::iterator::operator--(int) {
	DList<TYPE>::iterator orig(data);
	
	data = data->prev;
	
	return orig;
}


template <typename TYPE> 
typename DList<TYPE>::iterator& DList<TYPE>::iterator::operator=(const iterator& it) {
	data = it.data;
}


template <typename TYPE> 
typename DList<TYPE>::iterator& DList<TYPE>::iterator::operator-(int scale) {
	scale = abs(scale);
	
	while(scale --> 0)
		data = data->prev;

	return *this;
}


template <typename TYPE> 
typename DList<TYPE>::iterator& DList<TYPE>::iterator::operator+(int scale) {
	scale = abs(scale);
	
	while(scale --> 0)
		data = data->next;

	return *this;
}


template <typename TYPE> 
bool DList<TYPE>::iterator::operator==(const iterator& it) {
	return data == it.data;
}


template <typename TYPE> 
bool DList<TYPE>::iterator::operator!=(const iterator& it) {
	return !operator==(it);
}


template <typename TYPE> 
TYPE& DList<TYPE>::iterator::operator*() {
	return data->data;
}


template <typename TYPE> 
TYPE& DList<TYPE>::iterator::operator->() {
	return data->data;
}


template <typename TYPE> 
typename DList<TYPE>::Node* DList<TYPE>::iterator::getNode() {
	return data;
}
