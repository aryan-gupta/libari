
namespace me {
	template<typename T>
	struct greater {
		bool operator()(const T& a, const T& b) {
			return a > b;
		}
	}
	
	template<typename T>
	struct less {
		bool operator()(const T& a, const T& b) {
			return a < b;
		}
	}
}

// right now keep it as std::vector, when we finish DArray, we'll change that
template <typename TType, typename TCont = std::vector<TType>, typename TFunc = me::greater<TType>>
class Heap {
public:
	// class Iterator {};
	
	Heap();
	Heap(const Heap& other);
	Heap(Heap&& other);
	
	template <typename TIter>
	Heap(TIter begin, TIter end);
	
	Heap& operator=(const Heap& other);
	Heap& operator=(Heap&& other);
	
	void push(TType element);
	const T& top() const;
	void pop();
	size_t size() const;
	
private:
	void heapifyup(TCont::iterator idx);
	void heapifydown(TCont::iterator idx);
	
	TCont::iterator getLeftChIt(TCont::iterator idx);
	TCont::iterator getRightChIt(TCont::iterator idx);
	
	TCont mHeap;
	
};


