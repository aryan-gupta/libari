

#ifndef ARI_UNORDERED_MAP_HPP
#definf ARI_UNORDERED_MAP_HPP

namespace ari {

template <
	typename TKey,
	typename TType
> struct ump_node {
	using key_type = TKey;
	using mapped_type = TType;

	std::pair<const key_type, mapped_type> data;
	ump_node* next;
	
	// so we can call construct from allocator
	ump_node(const key_type key, mapped_type type, ump_node* node)
	    : data{key, type}, next{node} { /* No Code */ }
};


template <
	typename TKey,
	typename TType,
	typename THash = std::hash<TKey>,
	typename TKeyEq = std::equal_to<TKey>,
	typename TAlloc = std::allocator<std::pair<TKey, TType>>
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
	// using iterator             = ForwardIterator
	// using const_iterator       = Constant forward iterator
	// using local_iterator       = 
	// using const_local_iterator = 
	using node_type            = ump_node<key_type, mapped_type>;
	// using insert_return_type   =
	
	unordered_map();
	
	explicit unordered_map(
		size_type bucket_count,
		const hasher& hash = hasher{},
		const key_equal& equal = key_equal{},
		const allocator_type& alloc = allocator_type{}
	);
	
	unordered_map(size_type bucket_count, const allocator_type& alloc);
	unordered_map(size_type bucket_count, const hasher& hash, const allocator_type& alloc);
	explicit unordered_map(const allocator_type& alloc);
	
	template <typename TIter>
	unordered_map(
		TIter first, TIter last,
		size_type bucket_count = DEFAULT_BUCKET_COUNT,
		const hasher& hash = hasher{},
		const key_equal& equal = key_equal{},
		const allocator_type& alloc = allocator_type{}
	);
	
	template <typename TIter>
	unordered_map(TIter first, TIter last, size_type bucket_count, const allocator_type& alloc);
	
	template <typename TIter>
	unordered_map(TIter first, TIter last, size_type bucket_count, const hasher& hash, const allocator_type& alloc);
	
	unordered_map(const unordered_map& other);
	unordered_map(const unordered_map& other, const allocator_type& alloc);
	unordered_map(unordered_map&& other);
	unordered_map(unordered_map&& other, const allocator_type& alloc);
	
	unordered_map(
		std::initializer_list<value_type> init,
		size_type bucket_count = DEFAULT_BUCKET_COUNT,
		const hasher& hash = hasher{},
		const key_equalual& equal = key_equalual{},
		const allocator_type& alloc = allocator_type{}
	);


	unordered_map(
		std::initializer_list<value_type> init,
		size_type bucket_count,
		const allocator_type& alloc
	);


	unordered_map(
		std::initializer_list<value_type> init,
		size_type bucket_count,
		const hasher& hash,
		const allocator_type& alloc
	);
	
private:
	template <TIter>
	void mfAdd(TIter begin, TIter end);

	using node_allocator_type = typename allocator_type::rebind<node_type>::other;
	using node_type_pointer   = typename std::allocator_traits<node_allocator_type>::pointer;
	
	static const size_type DEFAULT_BUCKET_COUNT = 10;
	
	node_allocator_type mAlloc;
	size_type mNumBucket;
	size_type mSize;
	node_type_pointer mMap;
	hasher mHash;
	key_equal mEq;
	
};




template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map()
: unordered_map{DEFAULT_BUCKET_COUNT} // call other constructor


template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::size_type bucket_count,
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::hasher& hash,
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::key_equal& equal,
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::allocator_type& alloc /// @todo look in stl to see how this works
) : mAlloc{}, mNumBucket{bucket_count}, mSize{}, mMap{mAlloc.allocate(mNumBucket)},
    mHash{hash}, mEq{equal} { /* No Code */ }


template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::size_type bucket_count,
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::allocator_type& alloc 
) : mAlloc{}, mNumBucket{bucket_count}, mSize{}, mMap{mAlloc.allocate(mNumBucket)},
    mHash{}, mEq{} { /* No Code */ }
	
	
template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::size_type bucket_count,
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::hasher& hash,	
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::allocator_type& alloc
) : mAlloc{}, mNumBucket{bucket_count}, mSize{}, mMap{mAlloc.allocate(mNumBucket)},
    mHash{hash}, mEq{} { /* No Code */ }
	
	
template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::allocator_type& alloc
) : unordered_map{} { /* No Code */ }


template <TKey, TType, THash, TKeyEq, TAlloc>
template <TIter>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	TIter first, TIter last,
	typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::size_type bucket_count,
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::hasher& hash,
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::key_equal& equal,
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::allocator_type& alloc
) : mAlloc{}, mNumBucket{bucket_count}, mSize{std::distance(first, last)},
    mMap{mAlloc.allocate(mNumBucket)}, mHash{hash}, mEq{equal}
    { mfAdd(first, last); }
	
	
template <TKey, TType, THash, TKeyEq, TAlloc>
template <TIter>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	TIter first, TIter last,
	typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::size_type bucket_count,
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::allocator_type& alloc
) : mAlloc{}, mNumBucket{bucket_count}, mSize{std::distance(first, last)},
    mMap{mAlloc.allocate(mNumBucket)}, mHash{}, mEq{}
	{ mfAdd(first, last); }
	
	
template <TKey, TType, THash, TKeyEq, TAlloc>
template <TIter>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	TIter first, TIter last,
	typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::size_type bucket_count,
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::hasher& hash,
	const typename unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::allocator_type& alloc
) : mAlloc{}, mNumBucket{bucket_count}, mSize{std::distance(first, last)},
    mMap{mAlloc.allocate(mNumBucket)}, mHash{hash}, mEq{}
	{ mfAdd(first, last); }
	
	
template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(const)
	
	
template <TKey, TType, THash, TKeyEq, TAlloc>
template <TIter>
void unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::mfAdd(TIter begin, TIter end) {
	while (++first != last) {
		size_type idx = mHash(*first) % mNumBucket; // create hash
		
		if (mMap[idx] == nullptr)
			mMap[idx] = mAlloc.construct(first->first, first->second, nullptr);
		else {
			node_type_pointer current = mMap[idx].next;
			
			while (current->next != nullptr) {
				current = current->next;
			}
			
			current->next = mAlloc.construct(first->first, first->second, nullptr);
		}
	}
}
	
	
	
} // end namespace ari

#endif