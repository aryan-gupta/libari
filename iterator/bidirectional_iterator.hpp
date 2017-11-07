

#ifndef ARI_BIDIRECTIONAL_ITERATOR_HPP
#define ARI_BIDIRECTIONAL_ITERATOR_HPP


namespace ari {
	
template <typename TIter>
class bidirectional_iterator : public ari::iterator_base<
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
	
	bidirectional_iterator() : mCurrent{} { /* No Code */ }
	
	explicit bidirectional_iterator(const iterator_type& dat) : mCurrent{dat} { /* No Code */ }
	
	bidirectional_iterator(const bidirectional_iterator& it)
		: mCurrent{it.mCurrent} { /* No Code */ }
	
	template <typename TOther>
	bidirectional_iterator(const bidirectional_iterator<TOther>& it)
		: mCurrent{static_cast<TIter>(it.mCurrent)} { /* No Code */ }
		
	bidirectional_iterator& operator=(const bidirectional_iterator& it) const
		{ mCurrent = it.mCurrent; return *this; }
		
	
	
};

} // end namespace ari

#endif // ARI_BIDIRECTIONAL_ITERATOR_HPP defined