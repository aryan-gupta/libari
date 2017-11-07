

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
};


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
	using rvalue_reference     = rvalue_reference;
	using pointer              = typename std::allocator_traits<allocator_type>::pointer;
	using const_pointer        = typename std::allocator_traits<allocator_type>::const_pointer;
	// using iterator             = ForwardIterator
	// using const_iterator       = Constant forward iterator
	// using local_iterator       = 
	// using const_local_iterator = 
	using node_type            = ump_node<key_type, mapped_type>;
	// using insert_return_type   =
	using ratio_type           = float;
	
	unordered_map();
	
	explicit unordered_map(
		size_type bc,
		const hasher& hash          = hasher{},
		const key_equal& equal      = key_equal{},
		const allocator_type& alloc = allocator_type{}
	);
	
	unordered_map(size_type bc, const allocator_type& alloc);
	unordered_map(size_type bc, const hasher& hash, const allocator_type& alloc);
	explicit unordered_map(const allocator_type& alloc);
	
	template <typename TIter>
	unordered_map(
		TIter first, TIter last,
		size_type bc                = DEFAULT_BUCKET_COUNT,
		const hasher& hash          = hasher{},
		const key_equal& equal      = key_equal{},
		const allocator_type& alloc = allocator_type{}
	);
	
	template <typename TIter> unordered_map(TIter first, TIter last, size_type bc, const allocator_type& alloc);
	template <typename TIter> unordered_map(TIter first, TIter last, size_type bc, const hasher& hash, const allocator_type& alloc);
	unordered_map(const unordered_map& other);
	unordered_map(const unordered_map& other, const allocator_type& alloc);
	unordered_map(unordered_map&& other);
	unordered_map(unordered_map&& other, const allocator_type& alloc);
	
	unordered_map(
		std::initializer_list<value_type> init,
		size_type bc                            = DEFAULT_BUCKET_COUNT,
		const hasher& hash                      = hasher{},
		const key_equalual& equal               = key_equalual{},
		const allocator_type& alloc             = allocator_type{}
	);

	unordered_map(std::initializer_list<value_type> init, size_type bc, const allocator_type& alloc);
	unordered_map(std::initializer_list<value_type> init, size_type bc, const hasher& hash, const allocator_type& alloc);
	
	~unordered_map();
	
	unordered_map& operator=(const unordered_map& other);
	unordered_map& operator=(unordered_map&& other);
	unordered_map& operator=(std::initializer_list<value_type> ilist);
	
	allocator_type get_allocator() const;
	
	iterator begin();
	iterator end();
	const_iterator begin() const; 
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;
	
	bool empty() const;
	size_type size() const;
	size_type max_size() const;
	
	void clear();
	
	// insert
	std::pair<iterator, bool> insert(cont_reference val);
	template <typename P> std::pair<iterator, bool> insert(P&& val);
	std::pair<iterator, bool> insert(rvalue_reference val);
	iterator insert(const_iterator hint, const_reference val);
	template <typename P> iterator insert(const_iterator hint, rvalue_reference val);	
	iterator insert(const_iterator hint, rvalue_reference val);
	template <typename TIter> void insert(TIter first, TIter last);
	void insert(std::initializer_list<value_type> ilist);
	insert_return_type insert(node_type&& nh);
	iterator insert(const_iterator hint, node_type&& nh);
	
	// insert_or_assign
	template <typename M> std::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);
	template <typename M> std::pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);
	template <typename M> std::pair<iterator, bool> insert_or_assign(const_iterator hint, const key_type& k, M&& obj);
	template <typename M> std::pair<iterator, bool> insert_or_assign(const_iterator hint, key_type&& k, M&& obj);
	
	// emplace
	template <typename... TArgs> std::pair<iterator, bool> emplace(TArgs&&... args);
	template <typename... TArgs> std::pair<iterator, bool> emplace_hint(const_iterator hint, TArgs&&... args);
	template <typename... TArgs> std::pair<iterator, bool> try_emplace(const key_type& k, TArgs&&... args);
	template <typename... TArgs> std::pair<iterator, bool> try_emplace(key_type&& k, TArgs&&... args);
	template <typename... TArgs> std::pair<iterator, bool> try_emplace(const_iterator hint, const key_type& k, TArgs&&... args);
	template <typename... TArgs> std::pair<iterator, bool> try_emplace(const_iterator hint, key_type&& k, TArgs&&... args);
	
	// erase
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	iterator erase(const key_type& key);
	
	void swap(unordered_map& other);
	
	node_type extract(const_iterator pos);
	node_type extract(const key_type& k);
	
	template <typename THash2, typename TKeyEq2> void merge(const ari::unordered_map<TKey, TType, THash2, TKeyEq2, TAlloc>& source);
	template <typename THash2, typename TKeyEq2> void merge(ari::unordered_map<TKey, TType, THash2, TKeyEq2, TAlloc>&& source);
	// will do when we code a multimap
	// template <typename THash2, typename TKeyEq2> void merge(const ari::unordered_multimap<TKey, TType, THash2, TKeyEq2, TAlloc>& source);
	// template <typename THash2, typename TKeyEq2> void merge(ari::unordered_multimap<TKey, TType, THash2, TKeyEq2, TAlloc>&& source);
	
	// bucket interface
	local_iterator begin(size_type n);
	const_local_iterator begin(size_type n) const;
	const_local_iterator cbegin(size_type n) const;
	local_iterator end(size_type n);
	const_local_iterator end(size_type n) const;
	const_local_iterator cend(size_type n) const;
	size_type bucket_count() const;
	size_type max_bucket_count() const;
	size_type bucket_size(size_type n) const;
	size_type bucket(const key_type& key) const;
	
	ratio_type load_factor() const;
	ratio_type max_load_factor() const;
	void max_load_factor(float nlf);
	void rehash(size_type count);
	void reserve(size_type count);
	hasher hash_function() const;
	key_equal key_eq() const;	
	
private:
	using node_allocator_type    = typename allocator_type::rebind<node_type>::other;
	using node_pointer           = typename std::allocator_traits<node_allocator_type>::pointer;
	using node_pointer_allocator = typename allocator_type::rebind<node_pointer>::other;
	using node_pointer_array     = typename std::allocator_traits<node_allocator_type>::pointer;
	
	template <typename TIter> insert_base(TIter begin, TIter end);
	
	static const size_type DEFAULT_BUCKET_COUNT = 10;
	static const ratio_type DEFAULT_LOAD_FACTOR = 1.0;
	
	size_type mSize;
	size_type mBuckets;
	node_allocator_type mNodeAlloc;
	node_pointer_allocator mMapAlloc;
	node_pointer_array mMap;
	hasher mHash;
	key_equal mEq;
	ratio_type mLoadFactor;
	
};


template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map()
: unordered_map{DEFAULT_BUCKET_COUNT} { /* No Code */ }


template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	size_type bc,
	const hasher& hash,
	const key_equal& equal,
	const allocator_type& alloc
) : mSize{}, mBuckets{bc}, mAlloc{}, mMap{mAlloc.allocate(mBuckets)}, mHash{hash}, mEq{equal},
    mLoadFactor{DEFAULT_LOAD_FACTOR} { /* No Code */ }
	

template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	size_type bc,
	const allocator_type& alloc
) : mSize{}, mBuckets{bc}, mAlloc{alloc}, mMap{mAlloc.allocate(mBuckets)}, mHash{}, mEq{},
    mLoadFactor{DEFAULT_LOAD_FACTOR} { /* No Code */ }
	
	
template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	size_type bc,
	const hasher& hash,
	const allocator_type& alloc
) : mSize{}, mBuckets{bc}, mAlloc{alloc}, mMap{mAlloc.allocate(mBuckets)}, mHash{hash}, mEq{},
    mLoadFactor{DEFAULT_LOAD_FACTOR} { /* No Code */ }

	
template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	const allocator_type& alloc
) : mSize{}, mBuckets{DEFAULT_BUCKET_COUNT}, mAlloc{alloc}, mMap{mAlloc.allocate(mBuckets)}, mHash{},
    mEq{}, mLoadFactor{DEFAULT_LOAD_FACTOR} { /* No Code */ }
	
	
template <TKey, TType, THash, TKeyEq, TAlloc>
template <TIter>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	TIter first,
	TIter last,
	size_type bc,
	const hasher& hash,
	const key_equal& equal,
	const allocator_type& alloc
) : mSize{std::distance(first, last)}, mBuckets{mSize}, mAlloc{alloc}, mMap{mAlloc.allocate(mBuckets)},
    mHash{hash}, mEq{equal}, mLoadFactor{DEFAULT_LOAD_FACTOR} { insert_base(first, last); }
	
	
template <TKey, TType, THash, TKeyEq, TAlloc>
template <typename TIter>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(
	TIter first,
	TIter last,
	size_type bc,
	const allocator_type& alloc
) : mSize{std::distance(first, last)}, mBuckets{bc}, mAlloc{alloc},
    mMap{mAlloc.allocate(mBuckets)}, mHash{hash}, mEq{equal}, mLoadFactor{DEFAULT_LOAD_FACTOR} 
	{ insert_base(first, last); }
	
	
template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(const unordered_map& other)
: mSize{other.mSize}, mBuckets{other.mBuckets}, mAlloc{other.mAlloc}, mMap{mAlloc.allocate(mBuckets)},
  mHash{other.mHash}, mEq{other.mEq}, mLoadFactor{other.mLoadFactor}
	{ insert_base(other.begin(), other.end()); }
	
	
template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(const unordered_map& other, const allocator_type& alloc)
: mSize{other.mSize}, mBuckets{other.mBuckets}, mAlloc{alloc}, mMap{mAlloc.allocate(mBuckets)},
  mHash{other.mHash}, mEq{other.mEq}, mLoadFactor{other.mLoadFactor}
	{ insert_base(other.begin(), other.end()); }
	

/// TODO
template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(unordered_map&& other)
: mSize{other.mSize}, mBuckets{other.mBuckets}, mAlloc{other.mAlloc}, mMap{mAlloc.allocate(mBuckets)},
  mHash{other.mHash}, mEq{other.mEq}, mLoadFactor{other.mLoadFactor}
	{ insert_base(other.begin(), other.end()); }
	
	
template <TKey, TType, THash, TKeyEq, TAlloc>
unordered_map<TKey, TType, THash, TKeyEq, TAlloc>::unordered_map(unordered_map&& other, const allocator_type& alloc)
: mSize{other.mSize}, mBuckets{other.mBuckets}, mAlloc{alloc}, mMap{mAlloc.allocate(mBuckets)},
  mHash{other.mHash}, mEq{other.mEq}, mLoadFactor{other.mLoadFactor}
	{ insert_base(other.begin(), other.end()); }
	
/// END TODO


} // end namespace ari

#endif