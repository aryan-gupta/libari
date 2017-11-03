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

#ifndef ARI_LIST_HPP
#define ARI_LIST_HPP

namespace ari {

template <typename TType>
struct list_node {
	list_node* prev;
	TType data;
	list_node* next;
};



template <typename TType, typename TAlloc = std::allocator<TType>>
class list {
public:
	using value_type      = TType;
	using allocator_type  = TAlloc;
	using size_type       = size_t;
	using reference       = value_type&;	
	using const_reference = const value_type&;
	using pointer         = typename std::allocator_traits<allocator_type>::pointer;
	using const_pointer   = typename std::allocator_traits<allocator_type>::const_pointer;
	using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
	
private:
	using node_type            = list_node<value_type>;
	using node_allocator_type  = allocator_type::template rebind<node_type>::other;
	using node_pointer         = typename std::allocator_traits<node_allocator_type>::pointer;
	using const_node_pointer   = typename std::allocator_traits<node_allocator_type>::const_pointer;
	
public:	
	list();
	explicit list(const allocator_type& alloc);
	list(size_type count, const_reference val, const allocator_type& alloc = allocator_type{});
	list(size_type count);
	list(size_type count, const_reference val, const allocator_type& alloc);
	template <typename TIter> list(TIter begin, TIter end, const allocator_type& alloc = allocator_type{});
	list(const list& other);
	list(const list& other, const allocator_type& alloc);
	list(list&& other);
	list(list&& other, const allocator_type& alloc);
	list(std::initializer_list<value_type> init);
	
	~list();
	
	list& operator=(const list& other);
	list& operator=(list&& other); //? add noexcept?
	list& operator=(std::initializer_list<value_type> ilst);
	
	void assign(size_type count, const_reference val);
	template <typename TIter> assign(TIter begin, TIter end);
	void assign(std::initializer_list<value_type> ilst);
	
	allocator_type get_allocator() const; 
	
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;	
	
	iterator begin();
	iterator end();
	const_iterator cbegin() const;
	const_iterator cend() const;
	reverse_iterator rbegin();
	reverse_iterator rend();
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;
	
	std::pair<node_pointer, node_pointer> data();
	std::pair<const_node_pointer, const_node_pointer> data() const;
	
	bool empty() const;
	size_type size() const;
	size_type max_size() const;
	
	void clear();
	// insert
	iterator insert(const_iterator pos, const_reference val);
	iterator insert(const_iterator pos, value_type&& val);
	iterator insert(const_iterator pos, size_type count, const_reference value);
	template <typename TIter> iterator insert(const_iterator pos, TIter first, TIter last);
	iterator insert(const_iterator pos, std::initializer_list<value_type> ilst);
	
	// emplace
	template <typename TArgs...> emplace(const_iterator pos, TArgs&&... args);
	template <typename TArgs...> emplace_back(TArgs&&... args);
	template <typename TArgs...> emplace_front(TArgs&&... args);
	
	// erase
	iterator erase(const_iterator pos);
	iterator erase(const_iterator begin, const_iterator end);
	
	// push ends
	void push_back(const_reference val);
	void push_back(value_type&& val);
	void push_front(const_reference val);
	void push_front(value_type&& val);
	
	// pop ends
	void pop_back();
	void pop_front();
	
	void resize(size_type count);
	void resize(size_type count, const_reference val = val{});
	
	void swap(list& other);
	
	void merge(list& other);
	void merge(list&& other);
	template <typename TPred> void merge(list& other, TPred cmp);
	template <typename TPred> void merge(list&& other, TPred cmp);
	
	void splice(const_iterator pos, list& other);
	void splice(const_iterator pos, list&& other);
	void splice(const_iterator pos, list& other, const_iterator it);
	void splice(const_iterator pos, list&& other, const_iterator it);
	void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);
	void splice(const_iterator pos, list&& other, const_iterator first, const_iterator last);
	
	void remove(const_reference val);
	template <typename TUPred> void remove_if(TUPred pred);
	
	void reverse();
	
	void unique();
	template <TBPred> void unique(TBPred binp);

	void sort();
	template <TBPred> void sort(TBPred binp)
	
private:	
	node_pointer mHead;
	node_pointer mTail;
	size_type mSize;
	
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
	return iterator(mTail);
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
DList<TYPE>::iterator::iterator(Node* const node) {
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
