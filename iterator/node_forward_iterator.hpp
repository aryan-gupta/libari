

#ifndef ARI_NODE_FORWARD_ITERATOR_HPP
#define ARI_NODE_FORWARD_ITERATOR_HPP

#include <cstddef>
#include <iterator>

#include "iterator_base.hpp"
#include "../node_traits.hpp"

namespace ari {
	
template <typename TNode>
class node_forward_iterator : public ari::iterator_base<
	typename ari::node_traits<TNode>::value_type,
	typename ari::node_traits<TNode>::difference_type,
	typename ari::node_traits<TNode>::pointer,
	typename ari::node_traits<TNode>::reference
> {
protected:
	using traits_type       = ari::node_traits<TNode>;
	
public:
	using iterator_type     = TNode;
	
	using value_type        = typename traits_type::value_type;
	using difference_type   = typename traits_type::difference_type;
	using pointer           = typename traits_type::pointer;
	using reference         = typename traits_type::reference;
	
	using iterator_category = std::forward_iterator_tag; // ugh we need this for now, remove later
	
	node_forward_iterator() : mCurrent{} { /* No Code */ }
	
	explicit node_forward_iterator(const iterator_type& dat) : mCurrent{dat} { /* No Code */ }
	
	node_forward_iterator(const node_forward_iterator& it) : mCurrent{it.mCurrent} { /* No Code */ }
	
	template <typename TONode>
	node_forward_iterator(const node_forward_iterator<TONode>& it)
		: mCurrent{static_cast<iterator_type>(it.base())} { /* No Code */ }
		
	node_forward_iterator& operator=(const node_forward_iterator& it) {
		mCurrent = it.mCurrent;
		return *this;
	}
	
	/// @return a reference to the this, just incremented by one
	node_forward_iterator& operator++() { 
		mCurrent = mCurrent->next;
		return *this;
	}
	
	/// @return a copy to this, the original this will be incremented by one
	node_forward_iterator operator++(int) {
		node_forward_iterator tmp{mCurrent};
		mCurrent = mCurrent->next;
		return tmp;
	}
	
	/// @return reference to the underlying data
	reference operator*() const { return mCurrent->data; }
	
	/// @return pointer to the value
	pointer operator->() const { return &(operator*()); }
	
	/// @return if data of this is equal to other data
	bool operator==(const node_forward_iterator& it) const { return mCurrent == it.mCurrent; }
	
	// @return if the data is not equal to other data
	bool operator!=(const node_forward_iterator& it) const { return mCurrent != it.mCurrent; }
    
	/// @return mCurrent
	/// returns a const reference to the base of the iterator
	const iterator_type& base() const { return mCurrent; }

	/// @return mCurrent
	/// returns the base of the iterator
	iterator_type& base() { return mCurrent; }
	
	
private:
	iterator_type mCurrent;
	
};
	
}

#endif