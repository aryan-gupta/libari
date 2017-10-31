

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
	using size_type       = typename TCont::size_type;
	using reference       = typename TCont::reference;
	using const_reference = typename TCont::const_reference;

	priority_queue(TPred cmp);
	priority_queue(TCont cont);
	priority_queue(TCont cont, TPred cmp);
	priority_queue(std::initializer_list<TType> lst);
	priority_queue(std::initializer_list<TType> lst, TPred cmp);
	priority_queue(const priority_queue<TType, TCont, TPred>& other);
	priority_queue(priority_queue<TType, TCont, TPred>&& other);
	
	template <typename TIter>
	priority_queue(TIter begin, TIter end);
	
	template <typename TIter>
	priority_queue(TIter begin, TIter end, TPred cmp);
	
	priority_queue<TType, TCont, TPred>& operator=(const priority_queue<TType, TCont, TPred>& other);
	priority_queue<TType, TCont, TPred>& operator=(priority_queue<TType, TCont, TPred>&& other);
	
	
	void push(TType element);
	const_reference top() const;
	void pop();
	size_type size() const;
	bool empty() const;
	void swap(priority_queue<TType, TCont, TPred>& other);
	void priority_queueify();
	
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
	void priority_queueifyup(typename TCont::iterator idx);
	void priority_queueifydown(typename TCont::iterator idx);
	
	typename TCont::iterator getLeftChIt(typename TCont::iterator idx);
	typename TCont::iterator getRightChIt(typename TCont::iterator idx);
	typename TCont::iterator getParentIt(typename TCont::iterator idx);
	
	container_type mpriority_queue;
	value_compare mComp;
	
};


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(TPred cmp)
: mpriority_queue{}, mComp{cmp} { /* No Code */ }


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(TCont cont)
: mpriority_queue{cont}, mComp{} {
	priority_queueify();
}


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(TCont cont, TPred cmp)
: mpriority_queue{cont}, mComp{cmp} {
	priority_queueify();
}


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(std::initializer_list<TType> lst)
: mpriority_queue{lst.begin(), lst.end()}, mComp{} {
	priority_queueify();
}


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(std::initializer_list<TType> lst, TPred cmp)
: mpriority_queue{lst.begin(), lst.end()}, mComp{cmp} {
	priority_queueify();
}


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(const priority_queue<TType, TCont, TPred>& other) 
: mpriority_queue{other.mpriority_queue}, mComp{other.mComp} { /* No Code */ }


template <typename TType, typename TCont, typename TPred>
priority_queue<TType, TCont, TPred>::priority_queue(priority_queue<TType, TCont, TPred>&& other)
: mpriority_queue{other.mpriority_queue}, mComp{other.mComp} { /* No Code */ }


template <typename TType, typename TCont, typename TPred>
template <typename TIter>
priority_queue<TType, TCont, TPred>::priority_queue(TIter begin, TIter end)
: mpriority_queue{begin, end} {
	priority_queueify();
}


template <typename TType, typename TCont, typename TPred>
template <typename TIter>
priority_queue<TType, TCont, TPred>::priority_queue(TIter begin, TIter end, TPred cmp)
: mpriority_queue{begin, end}, mComp{cmp} {
	priority_queueify();
}


template <typename TType, typename TCont, typename TPred> // template parameter
priority_queue<TType, TCont, TPred>& // return type
priority_queue<TType, TCont, TPred>::operator=(const priority_queue<TType, TCont, TPred>& other) { // member function
	if (&other == this)
		return *this;
	
	mpriority_queue = other.mpriority_queue;
	mComp = other.mComp;
	
	return *this;
}


template <typename TType, typename TCont, typename TPred> // template parameter
priority_queue<TType, TCont, TPred>& // return type
priority_queue<TType, TCont, TPred>::operator=(priority_queue<TType, TCont, TPred>&& other) { // member function
	if (&other == this)
		return *this;
	
	mpriority_queue = other.mpriority_queue;
	mComp = other.mComp;
	
	return *this;
}


template <typename TType, typename TCont, typename TPred>
void priority_queue<TType, TCont, TPred>::priority_queueify() {
	for (
		auto b = mpriority_queue.begin() - 1, e = std::next(mpriority_queue.begin(), mpriority_queue.size() / 2 - 1);
		b != e; 
		--e) { // use reverse iterators
		priority_queueifydown(e);
	}
}


template <typename TType, typename TCont, typename TPred>
void priority_queue<TType, TCont, TPred>::push(TType element) {
	mpriority_queue.push_back(element);
	priority_queueifyup(mpriority_queue.end() - 1);
}


template <typename TType, typename TCont, typename TPred>
typename priority_queue<TType, TCont, TPred>::const_reference priority_queue<TType, TCont, TPred>::top() const {
	return mpriority_queue.front(); // make sure that front is available
	// for most containers
}


template <typename TType, typename TCont, typename TPred>
void priority_queue<TType, TCont, TPred>::pop() {
	mpriority_queue.front() = mpriority_queue.back();
	mpriority_queue.pop_back(); // change this to erase an iterator
	priority_queueifydown(mpriority_queue.begin());
}


template <typename TType, typename TCont, typename TPred>
typename priority_queue<TType, TCont, TPred>::size_type priority_queue<TType, TCont, TPred>::size() const {
	return mpriority_queue.size();
}


template <typename TType, typename TCont, typename TPred>
bool priority_queue<TType, TCont, TPred>::empty() const {
	return mpriority_queue.empty();
}


template <typename TType, typename TCont, typename TPred>
void priority_queue<TType, TCont, TPred>::swap(priority_queue<TType, TCont, TPred>& other) {
	mpriority_queue.swap(other.mpriority_queue);
}

//#if __cplusplus >= 201703L
#if __cplusplus >= 201500L

template <typename TType, typename TCont, typename TPred>
template <typename TFunc, typename... TArgs>
auto priority_queue<TType, TCont, TPred>::call_member(TFunc func, TArgs... args) {
	return std::invoke(func, mpriority_queue, std::forward<TArgs>(args)...);
}

#else

template <typename TType, typename TCont, typename TPred>
template <typename TRet, typename... TArgs>
auto priority_queue<TType, TCont, TPred>::call_member(TRet TCont::* member, TArgs... args) {
	return (mpriority_queue.* member) (args...);
}


template <typename TType, typename TCont, typename TPred>
template <typename TLamb, typename... TArgs>
auto priority_queue<TType, TCont, TPred>::call_member(TLamb lambda, TArgs... args) {
	return lambda(mpriority_queue, args...);
}

#endif

template <typename TType, typename TCont, typename TPred>
void priority_queue<TType, TCont, TPred>::priority_queueifyup(typename TCont::iterator idx) {
	// I need to check for copies, some of these things could be turned
	// into refeerences
	auto begin = mpriority_queue.begin(); // use reverse iterators
	auto pt = getParentIt(idx); 
	
	if (std::distance(begin, pt) >= 0 and mComp(*idx, *pt)) {
		std::iter_swap(idx, pt);
		priority_queueifyup(pt);
	}
}


template <typename TType, typename TCont, typename TPred>
void priority_queue<TType, TCont, TPred>::priority_queueifydown(typename TCont::iterator idx) {
	auto lc  = getLeftChIt(idx); 
	auto rc  = getRightChIt(idx);
	auto max = idx;
	auto end = mpriority_queue.end();
	
	if (std::distance(lc, end) > 0 and mComp(*lc, *max))
		max = lc;
	
	if (std::distance(rc, end) > 0 and mComp(*rc, *max))
		max = rc;
	
	if (idx != max) {
		std::iter_swap(idx, max);
		priority_queueifydown(max);
	}
}


template <typename TType, typename TCont, typename TPred>
typename TCont::iterator priority_queue<TType, TCont, TPred>::getLeftChIt(typename TCont::iterator idx) {
	return std::next(mpriority_queue.begin(), std::distance(mpriority_queue.begin(), idx) * 2 + 1);
}


template <typename TType, typename TCont, typename TPred>
typename TCont::iterator priority_queue<TType, TCont, TPred>::getRightChIt(typename TCont::iterator idx) {
	return std::next(mpriority_queue.begin(), std::distance(mpriority_queue.begin(), idx) * 2 + 2);
}


template <typename TType, typename TCont, typename TPred>
typename TCont::iterator priority_queue<TType, TCont, TPred>::getParentIt(typename TCont::iterator idx) {
	return std::next(mpriority_queue.begin(), (std::distance(mpriority_queue.begin(), idx) - 1) / 2);
}

} // end namespace ari

#endif // ARI_PRIORITY_QUEUE_HPP defined