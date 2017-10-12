
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


// right now keep it as std::vector, when we finish DArray, we'll change that
template <typename TType, typename TCont = std::vector<TType>, typename TFunc = me::greater<TType>>
class Heap {
public:
	// class Iterator {};
	
	Heap();
	Heap(TFunc cmp);
	Heap(const Heap<TType, TCont, TFunc>& other);
	Heap(const Heap<TType, TCont, TFunc>& other, TFunc cmp);
	Heap(Heap<TType, TCont, TFunc>&& other);
	Heap(Heap<TType, TCont, TFunc>&& other, TFunc cmp);
	
	template <typename TIter>
	Heap(TIter begin, TIter end);
	
	template <typename TIter>
	Heap(TIter begin, TIter end, TFunc cmp);
	
	Heap<TType, TCont, TFunc>& operator=(const Heap<TType, TCont, TFunc>& other);
	Heap<TType, TCont, TFunc>& operator=(Heap<TType, TCont, TFunc>&& other);
	
	void push(TType element);
	const TType& top() const;
	void pop();
	size_t size() const;
	bool empty() const;
	void swap(Heap<TType, TCont, TFunc>& other);
	
private:
	void heapifyup(typename TCont::iterator idx);
	void heapifydown(typename TCont::iterator idx);
	
	typename TCont::iterator getLeftChIt(typename TCont::iterator idx);
	typename TCont::iterator getRightChIt(typename TCont::iterator idx);
	typename TCont::iterator getParentIt(typename TCont::iterator idx);
	
	TCont mHeap;
	TFunc mComp;
	
};



template <typename TType, typename TCont, typename TFunc>
Heap<TType, TCont, TFunc>::Heap()
: mComp{} { /* No Code */ }


template <typename TType, typename TCont, typename TFunc>
Heap<TType, TCont, TFunc>::Heap(TFunc cmp)
: mComp{cmp} { /* No Code */ }


template <typename TType, typename TCont, typename TFunc>
Heap<TType, TCont, TFunc>::Heap(const Heap<TType, TCont, TFunc>& other) 
: mHeap{other}, mComp{} { /* No Code */ }


template <typename TType, typename TCont, typename TFunc>
Heap<TType, TCont, TFunc>::Heap(const Heap<TType, TCont, TFunc>& other, TFunc cmp) 
: mHeap{other}, mComp{cmp} { /* No Code */ }


template <typename TType, typename TCont, typename TFunc>
Heap<TType, TCont, TFunc>::Heap(Heap<TType, TCont, TFunc>&& other)
: mHeap{other} { /* No Code */ }


template <typename TType, typename TCont, typename TFunc>
Heap<TType, TCont, TFunc>::Heap(Heap<TType, TCont, TFunc>&& other, TFunc cmp)
: mHeap{other}, mComp{cmp} { /* No Code */ }


template <typename TType, typename TCont, typename TFunc>
template <typename TIter>
Heap<TType, TCont, TFunc>::Heap(TIter begin, TIter end)
: mHeap{begin, end} {
	for (
		auto b = mHeap.begin(), e = std::next(mHeap.begin(), mHeap.size() / 2);
		b != e; 
		++b) { // make names better
		heapifydown(b);
	}
}


template <typename TType, typename TCont, typename TFunc>
template <typename TIter>
Heap<TType, TCont, TFunc>::Heap(TIter begin, TIter end, TFunc cmp)
: mHeap{begin, end}, mComp{cmp} {
	for (
		auto b = mHeap.begin(), e = std::next(mHeap.begin(), mHeap.size() / 2);
		b != e; 
		++b) { // make names better
		heapifydown(b);
	}
}


template <typename TType, typename TCont, typename TFunc> // template parameter
Heap<TType, TCont, TFunc>& // return type
Heap<TType, TCont, TFunc>::operator=(const Heap<TType, TCont, TFunc>& other) { // member function
	if (&other == this)
		return *this;
	
	mHeap = other.mHeap;
	mComp = other.mComp;
	
	return *this;
}


template <typename TType, typename TCont, typename TFunc> // template parameter
Heap<TType, TCont, TFunc>& // return type
Heap<TType, TCont, TFunc>::operator=(Heap<TType, TCont, TFunc>&& other) { // member function
	if (&other == this)
		return *this;
	
	mHeap = other.mHeap;
	mComp = other.mComp;
	
	return *this;
}


template <typename TType, typename TCont, typename TFunc>
void Heap<TType, TCont, TFunc>::push(TType element) {
	mHeap.push_back(element);
	heapifyup(mHeap.begin());
}


template <typename TType, typename TCont, typename TFunc>
const TType& Heap<TType, TCont, TFunc>::top() const {
	return mHeap.front(); // make sure that front is available
	// for most containers
}


template <typename TType, typename TCont, typename TFunc>
void Heap<TType, TCont, TFunc>::pop() {
	mHeap.front() = mHeap.back();
	mHeap.pop_back(); // change this to erase an iterator
	heapifydown(mHeap.begin());
}


template <typename TType, typename TCont, typename TFunc>
size_t Heap<TType, TCont, TFunc>::size() const {
	return mHeap.size();
}


template <typename TType, typename TCont, typename TFunc>
bool Heap<TType, TCont, TFunc>::empty() const {
	return mHeap.empty();
}


template <typename TType, typename TCont, typename TFunc>
void Heap<TType, TCont, TFunc>::swap(Heap<TType, TCont, TFunc>& other) {
	mHeap.swap(other.mHeap);
}


template <typename TType, typename TCont, typename TFunc>
void Heap<TType, TCont, TFunc>::heapifyup(typename TCont::iterator idx) {
	// I need to check for copies, some of these things could be turned
	// into refeerences
	auto end = mHeap.end();
	auto pt = getParentIt(idx); 
	
	if (std::distance(idx, mHeap.end()) > 0 and mComp(*idx, *pt)) {
		std::iter_swap(idx, pt);
		heapifyup(pt);
	}
}


template <typename TType, typename TCont, typename TFunc>
void Heap<TType, TCont, TFunc>::heapifydown(typename TCont::iterator idx) {
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


template <typename TType, typename TCont, typename TFunc>
typename TCont::iterator Heap<TType, TCont, TFunc>::getLeftChIt(typename TCont::iterator idx) {
	return std::next(mHeap.begin(), std::distance(mHeap.begin(), idx) * 2 + 1);
}


template <typename TType, typename TCont, typename TFunc>
typename TCont::iterator Heap<TType, TCont, TFunc>::getRightChIt(typename TCont::iterator idx) {
	return std::next(mHeap.begin(), std::distance(mHeap.begin(), idx) * 2 + 2);
}


template <typename TType, typename TCont, typename TFunc>
typename TCont::iterator Heap<TType, TCont, TFunc>::getParentIt(typename TCont::iterator idx) {
	return std::next(mHeap.begin(), (std::distance(mHeap.begin(), idx) - 1) / -2);
}