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

template <typename TType, TAlloc>
struct node_type {
	node_pointer prev;
	TType data;
	node_pointer next;
};



template <typename TType, TAlloc, typename TAlloc = std::allocator<TType>>
class list {
public:
	using value_type      = TType;
	using allocator_type  = TAlloc;
	using size_type       = size_type;
	using reference       = value_type&;	
	using const_reference = const value_type&;
	using pointer         = typename std::allocator_traits<allocator_type>::pointer;
	using const_pointer   = typename std::allocator_traits<allocator_type>::const_pointer;
	using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
	
private:
	using node_type            = node_type<value_type>;
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

template <typename TType, TAlloc> 
list<TType, TAlloc>::list() {
	mHead = nullptr;
	mTail = nullptr;
	mSize = 0;
}


template <typename TType, TAlloc> 
list<TType, TAlloc>::list(const list& other) {
	mHead = nullptr;
	mSize = 0;
	
	if(other.size() > 0) {
		mHead = new node_type{nullptr, other[0], nullptr};
		mSize++;
	}
	
	node_pointer current = mHead, *prev = nullptr;
	
	for(size_type i = 1; i < other.size(); ++i) {
		prev = current;
		current = new node_type{prev, other[i], nullptr};
		prev->next = current;
		
		mSize++;
	}
	
	mTail = current;
}


template <typename TType, TAlloc> 
list<TType, TAlloc>::list(const size_type size, const TType& val) {
	mHead = nullptr;
	mSize = 0; /// @todo make one statement later rather than incrementing everytime
	
	if(size > 0) {
		mHead = new node_type{nullptr, val, nullptr};
		mSize++;
	}
	
	node_pointer current = mHead, *prev = nullptr;
	
	for(size_type i = 0; i < size; ++i) {
		prev = current;
		current = new node_type{prev, val, nullptr};
		prev->next = current;
		
		mSize++;
	}
	
	mTail = current;
}

template <typename TType, TAlloc>
template <typename TIter, typename>
list<TType, TAlloc>::list(TIter&& begin, TIter&& end) {
	mHead = nullptr;
	mSize = 0;
	
	if(std::distance(begin, end) > 0) {
		mHead = new node_type{nullptr, *begin++, nullptr};
		mSize++;
	}
	
	node_pointer current = mHead, *prev = nullptr;
	
	while(begin != end) {
		prev = current;
		current = new node_type{prev, *begin++, nullptr};
		prev->next = current;
		
		mSize++;
	}
	
	mTail = current;
}


template <typename TType, TAlloc> 
typename list<TType, TAlloc>::iterator list<TType, TAlloc>::begin() const {
	return iterator(mHead);
}


template <typename TType, TAlloc> 
typename list<TType, TAlloc>::iterator list<TType, TAlloc>::end() const {
	return iterator(mTail);
}


template <typename TType, TAlloc> 
void list<TType, TAlloc>::clear() {
	while(mHead != nullptr) {
		node_pointer rem = mHead;
		mHead = mHead->next;
		delete rem;
	}
	mHead = nullptr;
	mTail = nullptr;
	
	mSize = 0;
}


template <typename TType, TAlloc> 
void list<TType, TAlloc>::push_back(const TType& val) {
	if(mHead == nullptr && mTail == nullptr)
		mHead = mTail = new node_type{nullptr, val, nullptr};
	else
		mTail = mTail->next = new node_type{mTail, val, nullptr};
	
	mSize++;
}


template <typename TType, TAlloc> 
void list<TType, TAlloc>::push_front(const TType& val) {
	if(mHead == nullptr && mTail == nullptr)
		mHead = mTail = new node_type{nullptr, val, nullptr};
	else
		mHead = mHead->prev = new node_type{nullptr, val, mHead};
	
	mSize++;
}


template <typename TType, TAlloc> 
void list<TType, TAlloc>::pop_back() {
	if(mSize == 0)
		throw std::out_of_range("No elements to remove");
	
	node_pointer rem = mTail;
	mTail = mTail->prev;
	delete rem;
	mTail->next = nullptr;
	
	mSize--;
	if(mSize == 0) /// @todo Check mSize is 1 condition (do we need to change mTail to nullptr?)
		mHead = nullptr;
}


template <typename TType, TAlloc> 
void list<TType, TAlloc>::pop_front() {
	if(mSize == 0)
		throw std::out_of_range("No elements to remove");
	
	node_pointer rem = mHead;
	mHead = mHead->next;
	delete rem;
	mHead->prev = nullptr;
	
	mSize--;
	if(mSize == 0)
		mTail = nullptr;
}


template <typename TType, TAlloc> 
void list<TType, TAlloc>::insert(size_type idx, const TType& val) {
	if(idx > mSize)
		throw std::out_of_range("Supplied index was out of range");
	
	if       (idx == 0) {
		push_front(val);
	} else if(idx == mSize) {
		push_back(val);
	} else {
		node_pointer current = mHead;
		
		while(idx --> 0)
			current = current->next;
		
		node_pointer newPrev = current->prev;
		current->prev = new node_type{newPrev, val, current};
		newPrev->next = current->prev;
		
		mSize++;
	}
}


template <typename TType, TAlloc> 
void list<TType, TAlloc>::insert(const list<TType, TAlloc>::iterator it, const TType& val) {
	node_pointer current = it.getnode_type();
	node_pointer newPrev = current->prev;
	
	current->prev = new node_type{newPrev, val, current};
	
	mSize++;
}


template <typename TType, TAlloc> 
void list<TType, TAlloc>::remove(const TType& val) {
	node_pointer current = mHead, *rem = nullptr;
	
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


template <typename TType, TAlloc> 
size_type list<TType, TAlloc>::size() const {
	return mSize;
}


template <typename TType, TAlloc> 
const TType& list<TType, TAlloc>::operator[](size_type idx) const {
	node_pointer current = mHead;
	
	while(idx --> 0)
		current = current->next;
	
	return current->data;
}


template <typename TType, TAlloc> 
TType& list<TType, TAlloc>::operator[](size_type idx) {
	node_pointer current = mHead;
	
	while(idx --> 0)
		current = current->next;
	
	return current->data;
}

} // end namespace ari

#endif // ARI_LIST_HPP defined