


#ifndef ARI_RANDOM_ACCESS_ITERATOR_H
#define ARI_RANDOM_ACCESS_ITERATOR_H

#include <cstddef>
#include <iterator>

#include "iterator_base.hpp"

namespace ari {

template <typename TIter>
class random_access_iterator : public ari::iterator_base<
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
	
	/// Default constructor
	/// Intializes \c mCurrent to \c 0 or \c nullptr if its a pointer
	random_access_iterator() : mCurrent{} { /* No Code */ }
	
	/// Covert Constructor
	/// This converts a iterator_type to this type
	explicit random_access_iterator(const iterator_type& dat) : mCurrent{dat} { /* No Code */ }
	
	/// Copy constructor
	/// Copies the underling data structure
	/// There is no need for move constructor
	random_access_iterator(const random_access_iterator& it) 
		: mCurrent{it.mCurrent} { /* No Code */ }
	
	/// Copy constructor
	/// This attempts to copy another iterator if it can be converted
	template<typename TOther>
	random_access_iterator(const random_access_iterator<TOther>& it) 
		: mCurrent{static_cast<TIter>(it.mCurrent)} { /* No Code */ }
	
	/// Assignment operator
	/// Copies the underlying data over
	random_access_iterator& operator=(const random_access_iterator& it) {
		mCurrent = it.mCurrent;
	}
	
	/// @return if the underlyng data is less than the other
	bool operator<(const random_access_iterator& it) const { return mCurrent < it.mCurrent; }
	
	/// @return if the underlying data is greater than the other
	bool operator>(const random_access_iterator& it) const { return mCurrent > it.mCurrent; }
	
	/// @return if the underlyng data is less than or equal the other
	bool operator<=(const random_access_iterator& it) const { return mCurrent <= it.mCurrent; }
	
	/// @return if the underlying data is greater than or equal the other
	bool operator>=(const random_access_iterator& it) const { return mCurrent >= it.mCurrent; }
	
	/// @return a reference to the this, just incremented by one
	random_access_iterator& operator++() { 
		++mCurrent;
		return *this;
	}
	
	/// @return a reference to the this, just decremented by one
	random_access_iterator& operator--() {
		--mCurrent;
		return *this;
	}
	
	/// @return a copy to this, the original this will be incremented by one
	random_access_iterator operator++(int) { return random_access_iterator{mCurrent++}; }
	
	/// @return a copy to this, the original this will be decremented by one
	random_access_iterator operator--(int) { return random_access_iterator{mCurrent--}; }
	
	/// @return reference to \c this
	/// This operator decrements mCurrent by the scale
	random_access_iterator& operator-(difference_type scale) {
		mCurrent -= scale;
		return *this;
	}
	
	/// @return reference to \c this
	// This operator increments mCurrent by the scale
	random_access_iterator& operator+(difference_type scale) {
		mCurrent += scale;
		return *this;
	}
	
	/// @return reference to the underlying data
	reference operator*() const { return *mCurrent; }
	
	/// @return pointer to the value
	pointer operator->() const { return &(operator*()); }
	
	/// @return if data of this is equal to other data
	bool operator==(const random_access_iterator& it) const { return mCurrent == it.mCurrent; }
	
	// @return if the data is not equal to other data
	bool operator!=(const random_access_iterator& it) const { return mCurrent != it.mCurrent; }
    
	/// @return mCurrent
	/// returns the base of the iterator
	const iterator_type& base() const { return mCurrent; }

protected:
	iterator_type mCurrent;
	
};

} // end namespace ari

#endif // ARI_RANDOM_ACCESS_ITERATOR_H defined