

#ifndef ARI_UNORDERED_MAP_HPP
#definf ARI_UNORDERED_MAP_HPP

namespace ari {

template <
	typename TKey
	typename TType
	typename THash
	typename TKeyEq
	typename TAlloc
> class unordered_map {
public:
	using key_type             = TKey;
	using mapped_type          = TType;
	using value_type           = std::pair<const key_type, mapped_type>;
	using size_type            = size_t;
	using difference_type      = ptrdiff_t;
	using hasher               = THash;
	using key_equal            = TKeyEq;
	using allocator_type       = TAlloc;
	using reference            = value_type&;
	using const_reference      = const value_type&;
	using pointer              = typename std::allocator_traits<allocator_type>::pointer;
	using const_pointer        = typename std::allocator_traits<allocator_type>::const_pointer;
	/*using iterator             = ForwardIterator
	using const_iterator       = Constant forward iterator
	using local_iterator       = 
	using const_local_iterator = 
	using node_type            = 
	using insert_return_type   = */
	
	
private:

};

} // end namespace ari

#endif