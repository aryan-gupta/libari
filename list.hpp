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

#include <cstddef>
#include <memory>
#include <limits>

#include "Iterator.h"

namespace ari {

template <typename TType>
struct node_type {
	using value_type      = TType;
	using pointer         = TType*;
	using difference_type = ptrdiff_t;
	using reference       = TType&;
	
	using node_pointer    = node_type*;
	
	node_pointer prev;
	value_type   data;
	node_pointer next;
};


template <typename TType, typename TAlloc = std::allocator<TType>>
class list {
public:
	using value_type             = TType;
	using allocator_type         = TAlloc;
	using size_type              = size_type;
	using reference              = value_type&;	
	using const_reference        = const value_type&;
	using move_reference         = value_type&&;
	using pointer                = typename std::allocator_traits<allocator_type>::pointer;
	using const_pointer          = typename std::allocator_traits<allocator_type>::const_pointer;
	using difference_type        = typename std::allocator_traits<allocator_type>::difference_type;
	
private:
	using node_type              = node_type<value_type>;
	using node_allocator_type    = typename allocator_type::template rebind<node_type>::other;
	using node_pointer           = typename std::allocator_traits<node_allocator_type>::pointer;
	using const_node_pointer     = typename std::allocator_traits<node_allocator_type>::const_pointer;
	
public:	
	using iterator               = ari::random_access_iterator<node_pointer>;
	using const_iterator         = ari::random_access_iterator<const_node_pointer>;
	using reverse_iterator       = ari::random_access_iterator<const_node_pointer>;
	using const_reverse_iterator = ari::random_access_iterator<const_node_pointer>;
	
	
	list();
	explicit list(const allocator_type& alloc);
	list(size_type count, const_reference val, const allocator_type& alloc = allocator_type{});
	explicit list(size_type count);
	list(size_type count, const allocator_type& alloc = allocator_type{});
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
	template <typename TIter> void assign(TIter begin, TIter end);
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
	template <typename... TArgs> iterator emplace(const_iterator pos, TArgs&&... args);
	template <typename... TArgs> iterator emplace_back(TArgs&&... args);
	template <typename... TArgs> iterator emplace_front(TArgs&&... args);
	
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
	void resize(size_type count, const_reference val = {});
	
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
	template <typename TPred> void remove_if(TPred pred);
	
	void reverse();
	
	void unique();
	template <typename TPred> void unique(TPred binp);

	void sort();
	template <typename TPred> void sort(TPred binp);
	
private:
	node_pointer insert_base(node_pointer pos, const_reference val, size_type count = 1);
	node_pointer insert_base(node_pointer pos, value_type&& val);
	node_pointer insert_base(node_pointer pos, node_pointer obegin, node_pointer oend); // may soon be depreciated
	template <typename TIter> node_pointer insert_base(node_pointer pos, TIter begin, TIter end);
	
	node_pointer splice_base(node_pointer pos, node_pointer obegin, node_pointer oend);
	void merge_base(node_pointer obegin, node_pointer oend);
	
	node_pointer erase_base(node_pointer pos);
	node_pointer erase_base(node_pointer start, node_pointer end);
	
	node_allocator_type mAlloc;
	node_pointer mHead;
	node_pointer mTail;
	size_type mSize;
	
};


// IMPLEMENTATION

template <typename TType, typename TAlloc> 
list<TType, TAlloc>::list()
: mAlloc{}, mHead{}, mTail{}, mSize{} { /* No Code */ }


template <typename TType, typename TAlloc> 
list<TType, TAlloc>::list(const allocator_type& alloc)
: mAlloc{alloc}, mHead{}, mTail{}, mSize{} { /* No Code */ }


template <typename TType, typename TAlloc> 
list<TType, TAlloc>::list(size_type count, const_reference val, const allocator_type& alloc)
: mAlloc{alloc}, mHead{}, mTail{}, mSize{} { mTail = insert_base(mHead, val, count); }


template <typename TType, typename TAlloc> 
list<TType, TAlloc>::list(size_type count)
: mAlloc{}, mHead{}, mTail{}, mSize{} { mTail = insert_base(mHead, value_type{}, count); }


template <typename TType, typename TAlloc> 
list<TType, TAlloc>::list(size_type count, const allocator_type& alloc)
: mAlloc{alloc}, mHead{}, mTail{}, mSize{} { mTail = insert_base(mHead, value_type{}, count); }


template <typename TType, typename TAlloc> 
template <typename TIter>
list<TType, TAlloc>::list(TIter begin, TIter end, const allocator_type& alloc)
: mAlloc{alloc}, mHead{}, mTail{}, mSize{} { mTail = insert_base(mHead, begin, end); }


template <typename TType, typename TAlloc> 
list<TType, TAlloc>::list(const list& other)
: mAlloc{}, mHead{}, mTail{}, mSize{}
	{ mTail = insert_base(mHead, other.mHead, other.mTail); } // maybe convert to iterators? would remove one copy_base overload


template <typename TType, typename TAlloc> 
list<TType, TAlloc>::list(const list& other, const allocator_type& alloc)
: mAlloc{alloc}, mHead{}, mTail{}, mSize{}
	{ mTail = insert_base(mHead, other.mHead, other.mTail); }


template <typename TType, typename TAlloc> 
list<TType, TAlloc>::list(list&& other)
: mAlloc{}, mHead{}, mTail{}, mSize{}
	{ mTail = insert_base(mHead, other.mHead, other.mTail); }


template <typename TType, typename TAlloc> 
list<TType, TAlloc>::list(list&& other, const allocator_type& alloc)
: mAlloc{alloc}, mHead{}, mTail{}, mSize{}
	{ mTail = insert_base(mHead, other.mHead, other.mTail); }


template <typename TType, typename TAlloc> 
list<TType, TAlloc>::list(std::initializer_list<value_type> init)
: mAlloc{}, mHead{}, mTail{}, mSize{}
	{ mTail = insert_base(mHead, init.begin(), init.end()); }
	
	
template <typename TType, typename TAlloc> 
list<TType, TAlloc>::~list() { clear(); }


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::operator=(const list& other) -> list& {
	clear();
	mTail = insert_base(mHead, other.mHead, other.mTail);	
	return *this;
}


template <typename TType, typename TAlloc>
auto list<TType, TAlloc>::operator=(list&& other) -> list& {
	clear();
	mTail = splice_base(mHead, other.mHead, other.mTail);
	return *this;
}


template <typename TType, typename TAlloc>
auto list<TType, TAlloc>::operator=(std::initializer_list<value_type> init) -> list& {
	clear();
	mTail = insert_base(mHead, init.begin(), init.end());
	return *this;
}


template <typename TType, typename TAlloc>
void list<TType, TAlloc>::assign(size_type count, const_reference val) {
	clear();
	mTail = insert_base(mHead, val, count);
}


template <typename TType, typename TAlloc>
template <typename TIter>
void list<TType, TAlloc>::assign(TIter begin, TIter end) {
	clear();
	mTail = insert_base(mHead, begin, end);
}


template <typename TType, typename TAlloc>
void list<TType, TAlloc>::assign(std::initializer_list<value_type> ilst) {
	clear();
	mTail = insert_base(mHead, ilst.begin(), ilst.end());
}


template <typename TType, typename TAlloc>
auto list<TType, TAlloc>::get_allocator() const -> allocator_type { return allocator_type{mAlloc}; }


template <typename TType, typename TAlloc>
auto list<TType, TAlloc>::front() -> reference { return mHead->data; }


template <typename TType, typename TAlloc>
auto list<TType, TAlloc>::front() const -> const_reference { return mHead->data; }


template <typename TType, typename TAlloc>
auto list<TType, TAlloc>::back() -> reference { return mTail->data; }


template <typename TType, typename TAlloc>
auto list<TType, TAlloc>::back() const -> const_reference { return mTail->data; }


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::begin() -> iterator { return iterator{mHead}; }


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::end() -> iterator	{ return iterator{nullptr}; }


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::cbegin() const -> const_iterator { return const_iterator{mHead}; }


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::cend() const -> const_iterator { return const_iterator{nullptr}; }


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::rbegin() -> reverse_iterator { return reverse_iterator{mTail}; }


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::rend() -> reverse_iterator { return reverse_iterator{nullptr}; }


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::crbegin() const -> const_reverse_iterator
	{ return const_reverse_iterator{mTail}; }


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::crend() const -> const_reverse_iterator
	{ return const_reverse_iterator{nullptr}; }

	
template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::data() -> std::pair<node_pointer, node_pointer>
	{ return {mHead, mTail}; }

	
template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::data() const -> std::pair<const_node_pointer, const_node_pointer>
	{ return {mHead, mTail}; }
	
	
template <typename TType, typename TAlloc> 
bool list<TType, TAlloc>::empty() const { return mSize == 0; }


template <typename TType, typename TAlloc>
auto list<TType, TAlloc>::size() const -> size_type { return mSize; }


template <typename TType, typename TAlloc>
auto list<TType, TAlloc>::max_size() const -> size_type
	{ return std::numeric_limits<size_type>::max(); }


template <typename TType, typename TAlloc> ///@todo make sure that ease_base sets the 2 pointers to null
void list<TType, TAlloc>::clear() { erase_base(mHead, mTail); }


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::insert(const_iterator pos, const_reference val) -> iterator {
	// the reason we can do const_cast is because even though we might
	// do list<const T> the node is NOT a const only the data is. 
	// however, if we do const_cast on ari::vector<const T>
	// we will cause undefined behavior because we are removeing the 
	// const on the actual data (which is const). 
	insert_base(const_cast<node_pointer>(pos.base()), val);
}



template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::insert(const_iterator pos, value_type&& val) -> iterator {
	
}


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::insert(const_iterator pos, size_type count, const_reference val) -> iterator {
	
}


template <typename TType, typename TAlloc>
template <typename TIter> 
auto list<TType, TAlloc>::insert(const_iterator pos, TIter first, TIter last) -> iterator {
	
}


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::insert(const_iterator pos, std::initializer_list<value_type> ilst) -> iterator {
	
}


template <typename TType, typename TAlloc> 
template <typename... TArgs>
auto list<TType, TAlloc>::emplace(const_iterator pos, TArgs&&... args) -> iterator {
	
}


template <typename TType, typename TAlloc> 
template <typename... TArgs>
auto list<TType, TAlloc>::emplace_back(TArgs&&... args) -> iterator {
	
}


template <typename TType, typename TAlloc> 
template <typename... TArgs>
auto list<TType, TAlloc>::emplace_front(TArgs&&... args) -> iterator {
	
}


template <typename TType, typename TAlloc>
auto list<TType, TAlloc>::erase(const_iterator pos) -> iterator {
	
}


template <typename TType, typename TAlloc>
auto list<TType, TAlloc>::erase(const_iterator begin, const_iterator end) -> iterator {
	
}


template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::push_back(const_reference val) { insert_base(mTail, val); }


template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::push_back(value_type&& val) { insert_base(mTail, std::move(val)); }


template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::push_front(const_reference val) { insert_base(mHead, val); }


template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::push_front(value_type&& val) { insert_base(mHead, std::move(val)); }


// pop ends
template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::pop_back() {
	if(empty()) throw std::out_of_range("No elements to remove");
	erase_base(mTail);
}


template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::pop_front() {
	if(empty()) throw std::out_of_range("No elements to remove");
	erase_base(mHead);
}


template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::resize(size_type count) {
	
}


template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::resize(size_type count, const_reference val) {
	
}


template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::swap(list& other) {
	std::swap(mHead, other.mHead);
	std::swap(mTail, other.mTail);
}


/// @todo research why we need 2 of these, seems redundant
template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::merge(list& other) {
	merge_base(other.mHead->next, other.mTail);
}


template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::merge(list&& other) {
	merge_base(other.mHead->next, other.mTail);
}


template <typename TType, typename TAlloc> 
template <typename TPred>
void list<TType, TAlloc>::merge(list& other, TPred cmp) {
	
}


template <typename TType, typename TAlloc>
template <typename TPred>
void list<TType, TAlloc>::merge(list&& other, TPred cmp) {
	
}


// splice
/// @todo research more on what this does exactally 
template <typename TType, typename TAlloc>
void list<TType, TAlloc>::splice(const_iterator pos, list& other)
	{ insert_base(pos, other.mHead->next, other.mTail->prev); }


template <typename TType, typename TAlloc>
void list<TType, TAlloc>::splice(const_iterator pos, list&& other)
	{ splice_base(pos, other.mHead->next, other.mTail->prev); }


/// @tod This too. Research what all of these does
template <typename TType, typename TAlloc>
void list<TType, TAlloc>::splice(const_iterator pos, list& other, const_iterator it)
	{ splice_base(pos, it.base(), it.base()->next); }


template <typename TType, typename TAlloc>
void list<TType, TAlloc>::splice(const_iterator pos, list&& other, const_iterator it) {
	
}


template <typename TType, typename TAlloc>
void list<TType, TAlloc>::splice(const_iterator pos, list& other, const_iterator first, const_iterator last) {
	
}


template <typename TType, typename TAlloc>
void list<TType, TAlloc>::splice(const_iterator pos, list&& other, const_iterator first, const_iterator last) {
	
}


template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::remove(const TType& val) {
	node_pointer c = mHead->next;
	while (c != mTail) {
		if (c->data == val)
			c = erase_base(c);
		else
			c = c->next;
	}
}


template <typename TType, typename TAlloc>
template <typename TPred>
void list<TType, TAlloc>::remove_if(TPred pred) {
	node_pointer c = mHead->next;
	while (c != mTail) {
		if (pred(c->data))
			c = erase_base(c);
		else
			c = c->next;
	}
}


template <typename TType, typename TAlloc>
void list<TType, TAlloc>::reverse() {
	auto nums = mSize/2;
	auto begin = begin(), end = end();
	while (nums --> 0) {
		std::iter_swap(begin, end);
	}
}


template <typename TType, typename TAlloc>
void list<TType, TAlloc>::unique() {
	sort();
	std::unique(begin(), end());
}


template <typename TType, typename TAlloc>
template <typename TPred>
void list<TType, TAlloc>::unique(TPred binp) {
	sort();
	std::unique(begin(), end(), binp);
}


template <typename TType, typename TAlloc>
void list<TType, TAlloc>::sort() {
	std::sort(begin(), end());
}


template <typename TType, typename TAlloc>
template <typename TPred>
void list<TType, TAlloc>::sort(TPred binp) {
	std::sort(begin(), end(), binp);
}

/// @todo fix this to replicate the move insert_base function
/// @todo see if we can prevent code duplication on the while loop
template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::insert_base(node_pointer pos, const_reference val, size_type count) -> node_pointer {
	auto on = pos;
	pos = pos->prev;
	
	while (count --> 0) {
		pos->next = mAlloc.allocate(1);
		*(pos->next) = val;
		pos->next->prev = pos;
		pos = pos->next;
		++mSize;
	}
	
	pos->next = on;
}


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::insert_base(node_pointer pos, value_type&& val) -> node_pointer {
	auto n = mAlloc.allocate(1);
	*n = {pos->prev, std::move(val), pos};
	
	pos->prev = pos->prev->next = n;
	
	++mSize;
}


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::insert_base(node_pointer pos, node_pointer obegin, node_pointer oend) -> node_pointer {	
	auto on = pos;
	pos = pos->prev;
	
	while (obegin != oend) {
		pos->next = mAlloc.allocate(1);
		*(pos->next) = {pos, obegin->data, nullptr};
		pos = pos->next;
		obegin = obegin->next;
		++mSize;
	}
	
	pos->next = on;
}


template <typename TType, typename TAlloc> 
template <typename TIter>
auto list<TType, TAlloc>::insert_base(node_pointer pos, TIter begin, TIter end) {
	auto on = pos;
	pos = pos->prev;
	
	while (begin != end) {
		pos->next = mAlloc.allocate(1);
		*(pos->next) = {pos, *begin, nullptr};
		pos = pos->next;
		++begin;
		++mSize;
	}
	
	pos->next = on;
}


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::splice_base(node_pointer pos, node_pointer obegin, node_pointer oend) -> node_pointer {	
	auto prev = obegin;
	while (prev != oend)
		++mSize;
	
	prev = pos->prev;
	
	prev->next = obegin;
	pos->prev = oend->prev;
	pos->prev->next = pos;
	
	obegin->prev->next = oend;
	oend->prev = obegin->prev;
	obegin->prev = prev;
}


template <typename TType, typename TAlloc> 
void list<TType, TAlloc>::merge_base(node_pointer obegin, node_pointer oend) {
	auto begin = mHead->next;
	while (begin != mTail and obegin != oend) {
		if (obegin->data < begin->data) {
			begin->prev->next = ;
		}
	}
}


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::erase_base(node_pointer pos) -> node_pointer {
	pos->prev->next = pos->next;
	pos->next->prev = pos->prev;
	--mSize;
	
	auto ret = pos->next;
	
	mAlloc.deallocate(pos, 1);
	
	return ret;
}


template <typename TType, typename TAlloc> 
auto list<TType, TAlloc>::erase_base(node_pointer start, node_pointer end) -> node_pointer {
	start->prev->next = end;
	end->prev = start->prev;
	
	while (start != end) {
		auto del = start;
		start = start->next;
		mAlloc.deallocate(del, 1);
	}
	
	return end;	
}

} // end namespace ari

#endif // ARI_LIST_HPP defined