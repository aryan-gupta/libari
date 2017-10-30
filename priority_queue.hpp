

#ifndef ARI_HEAP_HPP
#define ARI_HEAP_HPP

namespace me {
	template<typename T>
	struct greater {
		bool operator()(const T& a, const T& b) {
			return a > b;
		}
	};
	
	template<typename T>
	struct less {
		bool operator()(const T& a, const T& b) {
			return a < b;
		}
	};
}

namespace ari {

/*
	TCont must support these members
	Most of these can be rewritten as iterators
	
	- TCont::begin
	- TCont::end
	- TCont::push_back
	- TCont::front
	- TCont::back
	- TCont::pop_back
	- TCont::size
	- TCont::empty
	- TCont::swap	
*/

// right now keep it as std::vector, when we finish DArray, we'll change that
template <typename TType, typename TCont = std::vector<TType>, typename TPred = me::greater<TType>>
class heap {
public:	
	using container_type  = TCont;
	using value_type      = TType;
	using value_compare   = TPred;
	using size_type       = typename TCont::size_type;
	using reference       = typename TCont::reference;
	using const_reference = typename TCont::const_reference;

	heap(TPred cmp);
	heap(TCont cont);
	heap(TCont cont, TPred cmp);
	heap(std::initializer_list<TType> lst);
	heap(std::initializer_list<TType> lst, TPred cmp);
	heap(const heap<TType, TCont, TPred>& other);
	heap(heap<TType, TCont, TPred>&& other);
	
	template <typename TIter>
	heap(TIter begin, TIter end);
	
	template <typename TIter>
	heap(TIter begin, TIter end, TPred cmp);
	
	heap<TType, TCont, TPred>& operator=(const heap<TType, TCont, TPred>& other);
	heap<TType, TCont, TPred>& operator=(heap<TType, TCont, TPred>&& other);
	
	
	void push(TType element);
	const_reference top() const;
	void pop();
	size_type size() const;
	bool empty() const;
	void swap(heap<TType, TCont, TPred>& other);
	void heapify();
	
//#if __cplusplus >= 201703L // becaise I'm using g++ 6.4 to compile
// https://stackoverflow.com/questions/38456127/what-is-the-value-of-cplusplus-for-c17
#if __cplusplus >= 201500L

	template <typename TFunc, typename... TArgs>
	auto call_member(TFunc func, TArgs... args);
	
#else
	
	template <typename TRet, typename... TArgs>
	auto call_member(TRet TCont::* member, TArgs... args);
	
	template <typename TLamb, typename... TArgs>
	auto call_member(TLamb lambda, TArgs... args);

#endif
	
private:
	void heapifyup(typename TCont::iterator idx);
	void heapifydown(typename TCont::iterator idx);
	
	typename TCont::iterator getLeftChIt(typename TCont::iterator idx);
	typename TCont::iterator getRightChIt(typename TCont::iterator idx);
	typename TCont::iterator getParentIt(typename TCont::iterator idx);
	
	container_type mheap;
	value_compare mComp;
	
};


template <typename TType, typename TCont, typename TPred>
heap<TType, TCont, TPred>::heap(TPred cmp)
: mheap{}, mComp{cmp} { /* No Code */ }


template <typename TType, typename TCont, typename TPred>
heap<TType, TCont, TPred>::heap(TCont cont)
: mheap{cont}, mComp{} {
	heapify();
}


template <typename TType, typename TCont, typename TPred>
heap<TType, TCont, TPred>::heap(TCont cont, TPred cmp)
: mheap{cont}, mComp{cmp} {
	heapify();
}


template <typename TType, typename TCont, typename TPred>
heap<TType, TCont, TPred>::heap(std::initializer_list<TType> lst)
: mheap{lst.begin(), lst.end()}, mComp{} {
	heapify();
}


template <typename TType, typename TCont, typename TPred>
heap<TType, TCont, TPred>::heap(std::initializer_list<TType> lst, TPred cmp)
: mheap{lst.begin(), lst.end()}, mComp{cmp} {
	heapify();
}


template <typename TType, typename TCont, typename TPred>
heap<TType, TCont, TPred>::heap(const heap<TType, TCont, TPred>& other) 
: mheap{other.mheap}, mComp{other.mComp} { /* No Code */ }


template <typename TType, typename TCont, typename TPred>
heap<TType, TCont, TPred>::heap(heap<TType, TCont, TPred>&& other)
: mheap{other.mheap}, mComp{other.mComp} { /* No Code */ }


template <typename TType, typename TCont, typename TPred>
template <typename TIter>
heap<TType, TCont, TPred>::heap(TIter begin, TIter end)
: mheap{begin, end} {
	heapify();
}


template <typename TType, typename TCont, typename TPred>
template <typename TIter>
heap<TType, TCont, TPred>::heap(TIter begin, TIter end, TPred cmp)
: mheap{begin, end}, mComp{cmp} {
	heapify();
}


template <typename TType, typename TCont, typename TPred> // template parameter
heap<TType, TCont, TPred>& // return type
heap<TType, TCont, TPred>::operator=(const heap<TType, TCont, TPred>& other) { // member function
	if (&other == this)
		return *this;
	
	mheap = other.mheap;
	mComp = other.mComp;
	
	return *this;
}


template <typename TType, typename TCont, typename TPred> // template parameter
heap<TType, TCont, TPred>& // return type
heap<TType, TCont, TPred>::operator=(heap<TType, TCont, TPred>&& other) { // member function
	if (&other == this)
		return *this;
	
	mheap = other.mheap;
	mComp = other.mComp;
	
	return *this;
}


template <typename TType, typename TCont, typename TPred>
void heap<TType, TCont, TPred>::heapify() {
	for (
		auto b = mheap.begin() - 1, e = std::next(mheap.begin(), mheap.size() / 2 - 1);
		b != e; 
		--e) { // use reverse iterators
		heapifydown(e);
	}
}


template <typename TType, typename TCont, typename TPred>
void heap<TType, TCont, TPred>::push(TType element) {
	mheap.push_back(element);
	heapifyup(mheap.end() - 1);
}


template <typename TType, typename TCont, typename TPred>
typename heap<TType, TCont, TPred>::const_reference heap<TType, TCont, TPred>::top() const {
	return mheap.front(); // make sure that front is available
	// for most containers
}


template <typename TType, typename TCont, typename TPred>
void heap<TType, TCont, TPred>::pop() {
	mheap.front() = mheap.back();
	mheap.pop_back(); // change this to erase an iterator
	heapifydown(mheap.begin());
}


template <typename TType, typename TCont, typename TPred>
typename heap<TType, TCont, TPred>::size_type heap<TType, TCont, TPred>::size() const {
	return mheap.size();
}


template <typename TType, typename TCont, typename TPred>
bool heap<TType, TCont, TPred>::empty() const {
	return mheap.empty();
}


template <typename TType, typename TCont, typename TPred>
void heap<TType, TCont, TPred>::swap(heap<TType, TCont, TPred>& other) {
	mheap.swap(other.mheap);
}

//#if __cplusplus >= 201703L
#if __cplusplus >= 201500L

template <typename TType, typename TCont, typename TPred>
template <typename TFunc, typename... TArgs>
auto heap<TType, TCont, TPred>::call_member(TFunc func, TArgs... args) {
	return std::invoke(func, mheap, std::forward<TArgs>(args)...);
}

#else

template <typename TType, typename TCont, typename TPred>
template <typename TRet, typename... TArgs>
auto heap<TType, TCont, TPred>::call_member(TRet TCont::* member, TArgs... args) {
	return (mheap.* member) (args...);
}


template <typename TType, typename TCont, typename TPred>
template <typename TLamb, typename... TArgs>
auto heap<TType, TCont, TPred>::call_member(TLamb lambda, TArgs... args) {
	return lambda(mheap, args...);
}

#endif

template <typename TType, typename TCont, typename TPred>
void heap<TType, TCont, TPred>::heapifyup(typename TCont::iterator idx) {
	// I need to check for copies, some of these things could be turned
	// into refeerences
	auto begin = mheap.begin(); // use reverse iterators
	auto pt = getParentIt(idx); 
	
	if (std::distance(begin, pt) >= 0 and mComp(*idx, *pt)) {
		std::iter_swap(idx, pt);
		heapifyup(pt);
	}
}


template <typename TType, typename TCont, typename TPred>
void heap<TType, TCont, TPred>::heapifydown(typename TCont::iterator idx) {
	auto lc  = getLeftChIt(idx); 
	auto rc  = getRightChIt(idx);
	auto max = idx;
	auto end = mheap.end();
	
	if (std::distance(lc, end) > 0 and mComp(*lc, *max))
		max = lc;
	
	if (std::distance(rc, end) > 0 and mComp(*rc, *max))
		max = rc;
	
	if (idx != max) {
		std::iter_swap(idx, max);
		heapifydown(max);
	}
}


template <typename TType, typename TCont, typename TPred>
typename TCont::iterator heap<TType, TCont, TPred>::getLeftChIt(typename TCont::iterator idx) {
	return std::next(mheap.begin(), std::distance(mheap.begin(), idx) * 2 + 1);
}


template <typename TType, typename TCont, typename TPred>
typename TCont::iterator heap<TType, TCont, TPred>::getRightChIt(typename TCont::iterator idx) {
	return std::next(mheap.begin(), std::distance(mheap.begin(), idx) * 2 + 2);
}


template <typename TType, typename TCont, typename TPred>
typename TCont::iterator heap<TType, TCont, TPred>::getParentIt(typename TCont::iterator idx) {
	return std::next(mheap.begin(), (std::distance(mheap.begin(), idx) - 1) / 2);
}

} // end namespace ari

#endif // ARI_HEAP_HPP defined