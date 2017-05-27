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
		
		TYPE& operator*();
		TYPE& operator->();
		
		Node* getNode();
		
	private:
		Node* data;
		
	};
	
	DList();
	DList(const DList& other);
	
	DList(const size_t size, const TYPE& val = TYPE());
	
	template <typename ITER>
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
template <typename ITER>
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
	while(mHead != mTail) {
		Node* rem = mHead;
		mHead = mHead->next;
		delete rem;
		--mSize; // only here for testing
		// will use below in final
	}
	mHead = nullptr;
	// mSize = 0; // Use this in final
}


template <typename TYPE> 
void DList<TYPE>::push_back(const TYPE& val) {
	mTail->next = new Node{mTail, val, nullptr};
	mTail = mTail->next;
	mSize++;
}


template <typename TYPE> 
void DList<TYPE>::push_front(const TYPE& val) {
	mHead->prev = new Node{nullptr, val, mHead};
	mHead = mHead->prev;
	mSize++;
}


template <typename TYPE> 
void DList<TYPE>::pop_back() {
	Node* rem = mTail;
	mTail = mTail->prev;
	delete rem;
	mSize--;
}


template <typename TYPE> 
void DList<TYPE>::pop_front() {
	Node* rem = mHead;
	mHead = mHead->next;
	delete rem;
	mSize--;
}


template <typename TYPE> 
void DList<TYPE>::insert(size_t idx, const TYPE& val) {
	Node* node = mHead;
	
	while(idx --> 0)
		node = node->next;
	
	Node* newNext = node->next;
	
	node->next = new Node{node, val, newNext};
	
	mSize++;
	
}


template <typename TYPE> 
void DList<TYPE>::insert(const DList<TYPE>::iterator it, const TYPE& val) {
	Node* current = it.getNode();
	Node* newNext = current->next;
	
	current->next = new Node{current, val, newNext};
	
	mSize++;
}


template <typename TYPE> 
void DList<TYPE>::remove(const TYPE& val) {
	Node* current = mHead, *rem = nullptr;
	
	while(current != nullptr) {
		if(current->data == val) {
			rem = current; /// @todo we can shorted this by a line
			current->prev->next = current->next;
			current->next->prev = current->prev;
			
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
