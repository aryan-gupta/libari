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
	class iterator {
	public:
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
		struct Node* data;
		
	};
	
	DList();
	DList(const DList& other);
	
	DList(const size_t size, const TYPE& val = TYPE());
	
	template <typename ITER>
	DList(const ITER& begin, const ITER& end);
	
	iterator begin() const;
	iterator end() const;
	
	void clear();
	
	void push_back(const TYPE& val);
	void push_front(const TYPE& val);
	
	void pop_back();
	void pop_front();
	
	void insert(size_t idx, const TYPE& val);
	void insert(const DList::iterator it, const TYPE& val);
	void remove(const TYPE& val);
	
	size_t size() const;
	
	const TYPE& operator[](size_t idx) const;
	TYPE& operator[](size_t idx);
	
private:
	struct Node {
		Node* prev;
		TYPE data;
		Node* next;
	};
	
	Node* mHead; /// @todo convert to iterators?
	Node* mTail;
	size_t mSize;
	
};


// IMPLEMENTATION

DList::DList() {
	mHead = nullptr;
	mTail = nullptr;
	mSize = 0;
}

DList::DList(const DList& other) { /// @todo set mHead to null in each constructor
	if(other.size() > 0)
		mHead = new Node{nullptr, other[0], nullptr};
	
	Node* current = mHead, prev = nullptr;
	
	for(size_t i = 1; i < other.size(); ++i) {
		prev = current;
		current = new Node{prev, other[i], nullptr};
		prev->next = current;
	}
	
	mTail = current;
}

DList::DList(const size_t size, const TYPE& val = TYPE()) {
	if(size > 0)
		mHead = new Node{nullptr, val, nullptr};
	
	Node* current = mHead, prev = nullptr;
	
	for(size_t i = 0; i < size; ++i) {
		prev = current;
		current = new Node{prev, val, nullptr};
		prev->next = current;
	}
	
	mTail = current;
}

template <typename ITER>
DList::DList(const ITER& begin, const ITER& end) {
	if(std::distance(start, end) > 0)
		mHead = new Node{nullptr, *begin++, nullptr};
	
	Node* current = mHead, prev = nullptr;
	
	while(begin != end) {
		prev = current;
		current = new Node{prev, *begin++, nullptr};
		prev->next = current;
	}
	
	mTail = current;
}

iterator DList::begin() {
	return iterator(mHead);
}

iterator DList::end() {
	/// @todo this part
	// maybe add tail member variable so we dont have 
	// to calculate it everytime we call this func?
}

void DList::clear() {
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

void DList::push_back(const TYPE& val) {
	mTail->next = new Node{mTail, val, nullptr};
	mTail = mTail->next;
	mSize++;
}

void DList::push_front(const TYPE& val) {
	mHead->prev = new Node{nullptr, val, mHead};
	mHead = mHead->prev;
	mSize++;
}

void DList::pop_back() {
	Node* rem = mTail;
	mTail = mTail->prev;
	delete rem;
	mSize--;
}

void DList::pop_front() {
	Node* rem = mHead;
	mHead = mHead->next;
	delete rem;
	mSize--;
}

void DList::insert(size_t idx, const TYPE& val) {
	Node* node = mHead;
	
	while(idx --> 0)
		node = node->next;
	
	Node* newNext = node->next;
	
	node->next = new Node{node, val, newNext};
	
	mSize++;
	
}

void DList::insert(const DList::iterator it, const TYPE& val) {
	Node* current = it.getNode();
	Node* newNext = current->next;
	
	current->next = new Node{current, val, newNext};
	
	mSize++;
}

void DList::remove(const TYPE& val) {
	Node* current = mHead, rem = nullptr;
	
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

size_t DList::size() {
	return mSize;
}

const TYPE& operator[](size_t idx) const {
	Node* current = mHead;
	
	while(idx --> 0)
		current = current->next;
	
	return current->data;
}

TYPE& operator[](size_t idx) {
	Node* current = mHead;
	
	while(idx --> 0)
		current = current->next;
	
	return current->data;
}

