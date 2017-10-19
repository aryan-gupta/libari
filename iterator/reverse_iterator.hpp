

#ifndef ARI_REVERSE_ITERATOR_H
#define ARI_REVERSE_ITERATOR_H

#include <cstddef>
#include <iterator>

#include "iterator_base.hpp"

namespace ari {

// Please note that alot of these operators will be defined outside the class
// This is because we can use this class to instatiate it for both 
// random_access_iterators and bidirectional_iterators. For example, if we
// instatiate this for bidirectional_iterator, the operator+(int) is not
// allowed, so the function (out-of-class definition) will not be used, however
// this operator is valid for reverse_iterator so it will generate code
// for it (technicality, if we use it). Se can use SFINAE, but right now the
// iterators dont have the typedef (using) for iterator_category
template <typename TIter>
class reverse_iterator : public ari::iterator_base<
	typename std::iterator_traits<TIter>::value_type,
	typename std::iterator_traits<TIter>::difference_type,
	typename std::iterator_traits<TIter>::pointer,
	typename std::iterator_traits<TIter>::reference
> {
protected:
	using traits_type = std::iterator_traits<TIter>;
	
public:
	using iterator_type     = TIter;
	
	using value_type        = typename traits_type::value_type;
	using difference_type   = typename traits_type::difference_type;
	using pointer           = typename traits_type::pointer;
	using reference         = typename traits_type::reference;
	
	reverse_iterator() : mCurrent{} { /* No Code */ }
	
	explicit reverse_iterator(iterator_type dat) : mCurrent{--dat} { /* No Code */ }
	
	reverse_iterator(const reverse_iterator& it)
		: mCurrent{it.mCurrent} { /* No Code */ }
		
	template <typename TOther>
	reverse_iterator(const reverse_iterator<TOther>& it)
		: mCurrent{static_cast<TIter>(it.mCurrent)} { /* No Code */ }
		
	reverse_iterator& operator=(const reverse_iterator& it) const { mCurrent = it.mCurrent; }
	
	/// @return a reference to the this, just incremented by one
	reverse_iterator& operator++() { 
		--mCurrent;
		return *this;
	}
	
	/// @return a reference to the this, just decremented by one
	reverse_iterator& operator--() {
		++mCurrent;
		return *this;
	}
	
	/// @return a copy to this, the original this will be incremented by one
	reverse_iterator operator++(int) { return reverse_iterator{mCurrent--}; }
	
	/// @return a copy to this, the original this will be decremented by one
	reverse_iterator operator--(int) { return reverse_iterator{mCurrent++}; }
	
	/// @return reference to the underlying data
	reference operator*() const { return *mCurrent; }
	
	/// @return pointer to the value
	pointer operator->() const { return &(operator*()); }
	
	/// @return if data of this is equal to other data
	bool operator==(const reverse_iterator& it) const { return mCurrent == it.mCurrent; }
	
	// @return if the data is not equal to other data
	bool operator!=(const reverse_iterator& it) const { return mCurrent != it.mCurrent; }
    
	/// @return mCurrent
	/// returns the base of the iterator
	const iterator_type& base() const { return mCurrent; }
	
protected:
	iterator_type mCurrent;
	
};


//////// These operators are ONLY valid for RandomAccessIterators ////////
	
/// @return if the underlyng data is less than the other
template <typename TIter>
bool operator<(const reverse_iterator<TIter>& a, const reverse_iterator<TIter>& b) 
	{ return a.base() < b.base(); }

/// @return if the underlying data is greater than the other
template <typename TIter>
bool operator>(const reverse_iterator<TIter>& a, const reverse_iterator<TIter>& b) 
	{ return a.base() > b.base(); }

/// @return if the underlyng data is less than or equal the other
template <typename TIter>
bool operator<=(const reverse_iterator<TIter>& a, const reverse_iterator<TIter>& b) 
	{ return a.base() <= b.base(); }

/// @return if the underlying data is greater than or equal the other
template <typename TIter>
bool operator>=(const reverse_iterator<TIter>& a, const reverse_iterator<TIter>& b) 
	{ return a.base() >= b.base(); }

/// @return reference to \c this
/// This operator decrements mCurrent by the scale
template <typename TIter>
reverse_iterator<TIter> operator-(
	const reverse_iterator<TIter>& it,
	typename reverse_iterator<TIter>::difference_type scale
) { return reverse_iterator<TIter>{it.base() - scale}; }

/// @return reference to \c this
// This operator increments mCurrent by the scale
template <typename TIter>
reverse_iterator<TIter> operator+(
	const reverse_iterator<TIter>& it,
	typename reverse_iterator<TIter>::difference_type scale
) { return reverse_iterator<TIter>{it.base() + scale}; }

template <typename TIter>
reverse_iterator<TIter>& operator+=(
	reverse_iterator<TIter>& it,
	typename reverse_iterator<TIter>::difference_type scale
) { it = it + scale; return it; }

template <typename TIter>
reverse_iterator<TIter>& operator-=(
	reverse_iterator<TIter>& it,
	typename reverse_iterator<TIter>::difference_type scale
) { it = it - scale; return it; }


template <typename TIter>
typename reverse_iterator<TIter>::difference_type operator-(
	const reverse_iterator<TIter>& a,
	const reverse_iterator<TIter>& b
) { return a.base() - b.base(); }


} // end namespace ari

#endif // ARI_REVERSE_ITERATOR_H defined