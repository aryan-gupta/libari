

#ifndef ARI_PRIORITY_QUEUE_HPP
#define ARI_PRIORITY_QUEUE_HPP

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
class priority_queue {
public:	
	using container_type  = TCont;
	using value_type      = TType;
	using value_compare   = TPred;
	using size_type       = typename container_type::size_type;
	using reference       = typename container_type::reference;
	using const_reference = typename container_type::const_reference;
	
	priority_queue(value_compare cmp);
	priority_queue(container_type cont);
	priority_queue(container_type cont, value_compare cmp);
	priority_queue(std::initializer_list<value_type> lst);
	priority_queue(std::initializer_list<value_type> lst, value_compare cmp);
	priority_queue(const priority_queue& other);
	priority_queue(priority_queue&& other);
	
	template <typename TIter>
	priority_queue(TIter begin, TIter end);
	
	template <typename TIter>
	priority_queue(TIter begin, TIter end, value_compare cmp);
	
	priority_queue& operator=(const priority_queue& other);
	priority_queue& operator=(priority_queue&& other);
	
	
	void push(value_type element);
	const_reference top() const;
	void pop();
	size_type size() const;
	bool empty() const;
	void swap(priority_queue& other);
	void priority_queueify();
	
//#if __cplusplus >= 201703L // becaise I'm using g++ 6.4 to compile
// https://stackoverflow.com/questions/38456127/what-is-the-value-of-cplusplus-for-c17
#if __cplusplus >= 201500L

	template <typename TFunc, typename... TArgs>
	auto call_member(TFunc func, TArgs... args);
	
#else
	
	template <typename TRet, typename... TArgs>
	auto call_member(TRet container_type::* member, TArgs... args);
	
	template <typename TLamb, typename... TArgs>
	auto call_member(TLamb lambda, TArgs... args);

#endif
	
private:
	using iterator = typename container_type::iterator;
	
	void heapifyup(iterator idx);
	void heapifydown(iterator idx);
	
	iterator getLeftChIt(iterator idx);
	iterator getRightChIt(iterator idx);
	iterator getParentIt(iterator idx);
	
	container_type mHeap;
	value_compare mComp;
	
};


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(value_compare cmp)
: mHeap{}, mComp{cmp} { /* No Code */ }


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(container_type cont)
: mHeap{cont}, mComp{} {
	priority_queueify();
}


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(container_type cont, value_compare cmp)
: mHeap{cont}, mComp{cmp} {
	priority_queueify();
}


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(std::initializer_list<value_type> lst)
: mHeap{lst.begin(), lst.end()}, mComp{} {
	priority_queueify();
}


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(std::initializer_list<value_type> lst, value_compare cmp)
: mHeap{lst.begin(), lst.end()}, mComp{cmp} {
	priority_queueify();
}


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(const priority_queue& other) 
: mHeap{other.mHeap}, mComp{other.mComp} { /* No Code */ }


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(priority_queue&& other)
: mHeap{other.mHeap}, mComp{other.mComp} { /* No Code */ }


template <typename TType, typename TCont, typename TPred>
template <typename TIter>
priority_queue<TType, TCont, TPred>::priority_queue(TIter begin, TIter end)
: mHeap{begin, end} {
	priority_queueify();
}


template <typename TType, typename TCont, typename TPred>
template <typename TIter>
priority_queue<TType, TCont, TPred>::priority_queue(TIter begin, TIter end, value_compare cmp)
: mHeap{begin, end}, mComp{cmp} {
	priority_queueify();
}


template <typename TType, typename TCont, typename TPred> 
auto priority_queue<TType, TCont, TPred>::operator=(const priority_queue& other) -> priority_queue& {
	if (&other == this)
		return *this;
	
	mHeap = other.mHeap;
	mComp = other.mComp;
	
	return *this;
}


template <typename TType, typename TCont, typename TPred>
auto priority_queue<TType, TCont, TPred>::operator=(priority_queue&& other) -> priority_queue& {
	if (&other == this)
		return *this;
	
	mHeap = other.mHeap;
	mComp = other.mComp;
	
	return *this;
}


template <typename TType, typename TCont, typename TPred>
void priority_queue<TType, TCont, TPred>::priority_queueify() {
	for (
		auto b = mHeap.begin() - 1, e = std::next(mHeap.begin(), mHeap.size() / 2 - 1);
		b != e; 
		--e) { // use reverse iterators
		heapifydown(e);
	}
}


template <typename TType, typename TCont, typename TPred>
void priority_queue<TType, TCont, TPred>::push(value_type element) {
	mHeap.push_back(element);
	heapifyup(mHeap.end() - 1);
}


template <typename TType, typename TCont, typename TPred>
auto priority_queue<TType, TCont, TPred>::top() const -> const_reference {
	return mHeap.front(); // make sure that front is available
	// for most containers
}


template <typename TType, typename TCont, typename TPred>
void priority_queue<TType, TCont, TPred>::pop() {
	mHeap.front() = mHeap.back();
	mHeap.pop_back(); // change this to erase an iterator
	heapifydown(mHeap.begin());
}


template <typename TType, typename TCont, typename TPred>
auto priority_queue<TType, TCont, TPred>::size() const -> size_type {
	return mHeap.size();
}


template <typename TType, typename TCont, typename TPred>
bool priority_queue<TType, TCont, TPred>::empty() const {
	return mHeap.empty();
}


template <typename TType, typename TCont, typename TPred>
void priority_queue<TType, TCont, TPred>::swap(priority_queue& other) {
	mHeap.swap(other.mHeap);
}

//#if __cplusplus >= 201703L
#if __cplusplus >= 201500L

template <typename TType, typename TCont, typename TPred>
template <typename TFunc, typename... TArgs>
auto priority_queue<TType, TCont, TPred>::call_member(TFunc func, TArgs... args) {
	return std::invoke(func, mHeap, std::forward<TArgs>(args)...);
}

#else

template <typename TType, typename TCont, typename TPred>
template <typename TRet, typename... TArgs>
auto priority_queue<TType, TCont, TPred>::call_member(TRet container_type::* member, TArgs... args) {
	return (mHeap.* member) (args...);
}


template <typename TType, typename TCont, typename TPred>
template <typename TLamb, typename... TArgs>
auto priority_queue<TType, TCont, TPred>::call_member(TLamb lambda, TArgs... args) {
	return lambda(mHeap, args...);
}

#endif

template <typename TType, typename TCont, typename TPred>
void priority_queue<TType, TCont, TPred>::heapifyup(iterator idx) {
	// I need to check for copies, some of these things could be turned
	// into refeerences
	auto begin = mHeap.begin(); // use reverse iterators
	auto pt = getParentIt(idx); 
	
	if (std::distance(begin, pt) >= 0 and mComp(*idx, *pt)) {
		std::iter_swap(idx, pt);
		heapifyup(pt);
	}
}


template <typename TType, typename TCont, typename TPred>
void priority_queue<TType, TCont, TPred>::heapifydown(iterator idx) {
	auto lc  = getLeftChIt(idx); 
	auto rc  = getRightChIt(idx);
	auto max = idx;
	auto end = mHeap.end();
	
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
auto priority_queue<TType, TCont, TPred>::getLeftChIt(iterator idx) -> iterator {
	return std::next(mHeap.begin(), std::distance(mHeap.begin(), idx) * 2 + 1);
}


template <typename TType, typename TCont, typename TPred>
auto priority_queue<TType, TCont, TPred>::getRightChIt(iterator idx) -> iterator {
	return std::next(mHeap.begin(), std::distance(mHeap.begin(), idx) * 2 + 2);
}


template <typename TType, typename TCont, typename TPred>
auto priority_queue<TType, TCont, TPred>::getParentIt(iterator idx) ->iterator {
	return std::next(mHeap.begin(), (std::distance(mHeap.begin(), idx) - 1) / 2);
}

} // end namespace ari

#endif // ARI_PRIORITY_QUEUE_HPP defined