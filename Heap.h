
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
class Heap {
public:	
	using container_type = TCont;
	using value_type = TType;
	using size_type = typename TCont::size_type;

	Heap(TPred cmp);
	Heap(TCont cont);
	Heap(TCont cont, TPred cmp);
	Heap(std::initializer_list<TType> lst);
	Heap(std::initializer_list<TType> lst, TPred cmp);
	Heap(const Heap<TType, TCont, TPred>& other);
	Heap(Heap<TType, TCont, TPred>&& other);
	
	template <typename TIter>
	Heap(TIter begin, TIter end);
	
	template <typename TIter>
	Heap(TIter begin, TIter end, TPred cmp);
	
	Heap<TType, TCont, TPred>& operator=(const Heap<TType, TCont, TPred>& other);
	Heap<TType, TCont, TPred>& operator=(Heap<TType, TCont, TPred>&& other);
	
	
	void push(TType element);
	const TType& top() const;
	void pop();
	size_t size() const;
	bool empty() const;
	void swap(Heap<TType, TCont, TPred>& other);
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
	
	TCont mHeap;
	TPred mComp;
	
};


template <typename TType, typename TCont, typename TPred>
Heap<TType, TCont, TPred>::Heap(TPred cmp)
: mHeap{}, mComp{cmp} { /* No Code */ }


template <typename TType, typename TCont, typename TPred>
Heap<TType, TCont, TPred>::Heap(TCont cont)
: mHeap{cont}, mComp{} {
	heapify();
}


template <typename TType, typename TCont, typename TPred>
Heap<TType, TCont, TPred>::Heap(TCont cont, TPred cmp)
: mHeap{cont}, mComp{cmp} {
	heapify();
}


template <typename TType, typename TCont, typename TPred>
Heap<TType, TCont, TPred>::Heap(std::initializer_list<TType> lst)
: mHeap{lst.begin(), lst.end()}, mComp{} {
	heapify();
}


template <typename TType, typename TCont, typename TPred>
Heap<TType, TCont, TPred>::Heap(std::initializer_list<TType> lst, TPred cmp)
: mHeap{lst.begin(), lst.end()}, mComp{cmp} {
	heapify();
}


template <typename TType, typename TCont, typename TPred>
Heap<TType, TCont, TPred>::Heap(const Heap<TType, TCont, TPred>& other) 
: mHeap{other.mHeap}, mComp{other.mComp} { /* No Code */ }


template <typename TType, typename TCont, typename TPred>
Heap<TType, TCont, TPred>::Heap(Heap<TType, TCont, TPred>&& other)
: mHeap{other.mHeap}, mComp{other.mComp} { /* No Code */ }


template <typename TType, typename TCont, typename TPred>
template <typename TIter>
Heap<TType, TCont, TPred>::Heap(TIter begin, TIter end)
: mHeap{begin, end} {
	heapify();
}


template <typename TType, typename TCont, typename TPred>
template <typename TIter>
Heap<TType, TCont, TPred>::Heap(TIter begin, TIter end, TPred cmp)
: mHeap{begin, end}, mComp{cmp} {
	heapify();
}


template <typename TType, typename TCont, typename TPred> // template parameter
Heap<TType, TCont, TPred>& // return type
Heap<TType, TCont, TPred>::operator=(const Heap<TType, TCont, TPred>& other) { // member function
	if (&other == this)
		return *this;
	
	mHeap = other.mHeap;
	mComp = other.mComp;
	
	return *this;
}


template <typename TType, typename TCont, typename TPred> // template parameter
Heap<TType, TCont, TPred>& // return type
Heap<TType, TCont, TPred>::operator=(Heap<TType, TCont, TPred>&& other) { // member function
	if (&other == this)
		return *this;
	
	mHeap = other.mHeap;
	mComp = other.mComp;
	
	return *this;
}


template <typename TType, typename TCont, typename TPred>
void Heap<TType, TCont, TPred>::heapify() {
	for (
		auto b = mHeap.begin() - 1, e = std::next(mHeap.begin(), mHeap.size() / 2 - 1);
		b != e; 
		--e) { // use reverse iterators
		heapifydown(e);
	}
}


template <typename TType, typename TCont, typename TPred>
void Heap<TType, TCont, TPred>::push(TType element) {
	mHeap.push_back(element);
	heapifyup(mHeap.end() - 1);
}


template <typename TType, typename TCont, typename TPred>
const TType& Heap<TType, TCont, TPred>::top() const {
	return mHeap.front(); // make sure that front is available
	// for most containers
}


template <typename TType, typename TCont, typename TPred>
void Heap<TType, TCont, TPred>::pop() {
	mHeap.front() = mHeap.back();
	mHeap.pop_back(); // change this to erase an iterator
	heapifydown(mHeap.begin());
}


template <typename TType, typename TCont, typename TPred>
size_t Heap<TType, TCont, TPred>::size() const {
	return mHeap.size();
}


template <typename TType, typename TCont, typename TPred>
bool Heap<TType, TCont, TPred>::empty() const {
	return mHeap.empty();
}


template <typename TType, typename TCont, typename TPred>
void Heap<TType, TCont, TPred>::swap(Heap<TType, TCont, TPred>& other) {
	mHeap.swap(other.mHeap);
}

//#if __cplusplus >= 201703L
#if __cplusplus >= 201500L

template <typename TType, typename TCont, typename TPred>
template <typename TFunc, typename... TArgs>
auto Heap<TType, TCont, TPred>::call_member(TFunc func, TArgs... args) {
	return std::invoke(func, mHeap, std::forward<TArgs>(args)...);
}

#else

template <typename TType, typename TCont, typename TPred>
template <typename TRet, typename... TArgs>
auto Heap<TType, TCont, TPred>::call_member(TRet TCont::* member, TArgs... args) {
	return (mHeap.* member) (args...);
}


template <typename TType, typename TCont, typename TPred>
template <typename TLamb, typename... TArgs>
auto Heap<TType, TCont, TPred>::call_member(TLamb lambda, TArgs... args) {
	return lambda(mHeap, args...);
}

#endif

template <typename TType, typename TCont, typename TPred>
void Heap<TType, TCont, TPred>::heapifyup(typename TCont::iterator idx) {
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
void Heap<TType, TCont, TPred>::heapifydown(typename TCont::iterator idx) {
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
typename TCont::iterator Heap<TType, TCont, TPred>::getLeftChIt(typename TCont::iterator idx) {
	return std::next(mHeap.begin(), std::distance(mHeap.begin(), idx) * 2 + 1);
}


template <typename TType, typename TCont, typename TPred>
typename TCont::iterator Heap<TType, TCont, TPred>::getRightChIt(typename TCont::iterator idx) {
	return std::next(mHeap.begin(), std::distance(mHeap.begin(), idx) * 2 + 2);
}


template <typename TType, typename TCont, typename TPred>
typename TCont::iterator Heap<TType, TCont, TPred>::getParentIt(typename TCont::iterator idx) {
	return std::next(mHeap.begin(), (std::distance(mHeap.begin(), idx) - 1) / 2);
}