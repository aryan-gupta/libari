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
		
		iterator& operator++(int); // postincrement
		iterator& operator--(int);
		
		iterator& operator=(const iterator& it);
		iterator& operator=(iterator&& it);
		
		iterator& operator-(const int scale);
		iterator& operator+(const int scale);
		
		bool operator==(const iterator& it);
		
		TYPE& operator*();
		TYPE& operator->();
		
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
	
	void insert(const size_t idx, const TYPE& val);
	void remove(const TYPE& val);
	
	size_t size() const;
	
	const TYPE& operator[](const size_t idx) const;
	TYPE& operator[](const size_t idx);
	
private:
	struct Node {
		Node* prev;
		TYPE data;
		Node* next;
	};
	
	Node* mHead;
	size_t mSize;
	
};


// IMPLEMENTATION

DList::DList() {
	mHead = nullptr;
	mSize = 0;
}

DList::DList(const DList& other) {
	Node* current = nullptr, prev = nullptr;
	
	if(other.size() > 0)
		current = new Node{nullptr, other[0], nullptr};
	
	mHead = current;
	
	for(size_t i = 1; i < other.size(); ++i) {
		prev = current;
		current = new Node{prev, other[i], nullptr};
		prev->next = current;
	}
	
	current->next = nullptr;
}

DList(const size_t size, const TYPE& val = TYPE()) {
	Node* current = nullptr, prev = nullptr;
	
	if(size > 0)
		current = new Node{nullptr, val, nullptr};
	
	mHead = current;
	
	for(size_t i = 0; i < size; ++i) {
		prev = current;
		current = new Node{prev, val, nullptr};
		prev->next = current;
	}
}