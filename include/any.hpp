
#include <exception>
#include <memory>
#include <typeinfo>
#include <type_traits>
#include <any> // for std::bad_any_cast

namespace ari {
	
using std::bad_any_cast;

/// @brief A class that can hold one of any type of data
class any {
	// friend classes, used so we can cast it to the proper type
	template <typename T> friend T* any_cast(any* a);
	template <typename T> friend const T* any_cast(const any* a);
	template <typename T> friend const T any_cast(const any& a);
	template <typename T> friend T any_cast(any& a);
	template <typename T> friend T any_cast(any&& a);
	
	
	// Dependent types for storage, this way its easier to increase the storage limitations
	// I have seen people use 2x the size of the pointer rather than 1x
	using heap_storage = void*;
	using stack_storage = std::aligned_storage_t<sizeof(void*), std::alignment_of_v<void*>>;
	
	
	/// @brief The main storage of ari::any
	/**
		If we are using heap storage, the actual opbject is created in the heap, and we
		store a pointer to the object. If we are using the object is small enough then 
		we can optimize this class by storing it in the stack. The the statck storage is
		just raw strage the size defined by the \c stack_storage type. 
	*/
	union AnyStorage {
		heap_storage  mPtr;
		stack_storage mBuf;
	};
	
	
	/// @brief A class that lets manage our AnyStorage union
	/**
		This class just has a bunch of function pointers to functions that define how to
		manage our data. For example, is out type was a char, then most likely we will be
		using stack strage (if \c requires_allocation is \c false), then the get function
		will point to \c AnyManagerStack<char>::get, and \c type_id will point to 
		\c AnyManagerStack<char>::type_id, and so on and forth. If our data was a struct 
		called \c BigType with a \c char[100] as a member, then we would use heap strage, so 
		our function pointer will point to \c AnyManagerHeap<BigType>::get. Please see 
		\c ari::any::get_manager<T> for more info. 
	*/
	struct AnyManager {
		void* (*get) (AnyStorage&);
		const std::type_info& (*type_id) ();
		AnyStorage (*clone) (const AnyStorage&);
		void (*destroy) (AnyStorage&);
		void (*move) (AnyStorage&, AnyStorage&);
		void (*swap) (AnyStorage& a, AnyStorage& b);
	};
	
	
	/// @brief The manager used for stack objects
	template <typename T>
	struct AnyManagerStack {
		
		template <typename... A>
		static AnyStorage create(A&&... args) {
			AnyStorage dest;
			::new (&dest.mBuf) T{ std::forward<A>(args)... };
			return dest;
		}
		
		static void* get(AnyStorage& dat) noexcept {
			return reinterpret_cast<T*>(&dat.mBuf);
		}
		
		static const std::type_info& type_id() noexcept {
			return typeid(T);
		}
		
		static AnyStorage clone(const AnyStorage& src) {
			AnyStorage dest;
			::new (&dest.mBuf) T{ reinterpret_cast<const T&>(src.mBuf) };
			return dest;
		}
		
		static void destroy(AnyStorage& dat) {
			reinterpret_cast<T*>(&dat.mBuf)->~T(); // call to destructor
		}
		
		static void move(AnyStorage& src, AnyStorage& dest) {
			::new (&dest.mBuf) T{ std::move(reinterpret_cast<T&>(src.mBuf)) };
		}
		
	};
	
	
	/// @brief The manager used for heap objects
	template <typename T>
	struct AnyManagerHeap {
		template <typename... A>
		static AnyStorage create(A&&... args) {
			AnyStorage dest;
			dest.mPtr = new T{ std::forward<A>(args)... };
			return dest; // RVO
		}
		
		static void* get(AnyStorage& dat) noexcept {
			return static_cast<T*>(dat.mPtr);
		}
		
		static const std::type_info& type_id() noexcept {
			return typeid(T);
		}
		
		static AnyStorage clone(const AnyStorage& src) {
			AnyStorage dest;
			dest.mPtr = new T{*static_cast<T*>(src.mPtr)};
			return dest;
		}
		
		static void destroy(AnyStorage& dat) {
			delete static_cast<T*>(dat.mPtr);
		}
		
		static void move(AnyStorage& src, AnyStorage& dest) {
			dest.mPtr = src.mPtr;
			src.mPtr = nullptr;
		}
		
	};
	
	
	/// @brief This defines if we need to allocate the object in the heap or not. 
	/// requires_allocation is true if we have a heap object (large object) and
	/// false if we can fit it in our stack_storage
	template <typename T>
	inline constexpr static bool requires_allocation =
		sizeof(T) >= sizeof(AnyStorage) and alignof(T) >= alignof(AnyStorage);
	
	
	/// @brief Uses the \c requires_allocation variable to decide which manager to use
	/// If we need allocation (heap object), manager_type is \c AnyManagerHeap<std::decay_t<T>>
	/// and vise versa
	template <typename T, bool b = requires_allocation<T>> 
	using manager_type = std::conditional_t<b, AnyManagerHeap<std::decay_t<T>>, AnyManagerStack<std::decay_t<T>>>;
	
	
	/// @brief Retrives the proper manager for \tparam T
	/// Because this is a static function, no matter how many any's are created, there will be only
	/// one function for each type (one get_manager<int>, one get_manager<char>, etc.). This way
	/// Also because AnyManager manager is also static, ever function will have one manager type
	/// in it. Overall each type we use, the compiler will automatically create our AnyManager.
	template <typename T>
	static AnyManager* get_manager() {
		using F = manager_type<T>;
		
		static AnyManager manager { &F::get, &F::type_id, &F::clone, &F::destroy, &F::move/*, &F::swap */};
		
		return &manager;
	}
	
	
	/// @brief Get's the type for type casting
	template <typename T> using any_cast_t = std::remove_cv_t<std::remove_reference_t<T>>;
	
	
	/// @brief Unsafely casts the internal storage
	template <typename T>
	T* get() {
		return static_cast<T*>(mManager->get(mStorage));
	}
	
	
	/// @brief Unsafely casts the internal storage
	template <typename T>
	const T* get() const {
		return static_cast<const T*>(mManager->get(mStorage));
	}
	
	
	// Private members
	AnyManager* mManager;
	AnyStorage  mStorage;
	
public:
	/// PLEASE SEE http://en.cppreference.com/w/cpp/utility/any FOR INFORMATION
	
	constexpr any() noexcept : mManager{ nullptr }, mStorage{ nullptr } {}
	
	any(const any& other) : mManager{ other.mManager }, mStorage{ mManager->clone(other.mStorage) } {}
	
	any(any&& other) noexcept : mManager { other.mManager } {
		mManager->move(other.mStorage, this->mStorage);
		other.mManager = nullptr;
	}
	
	template <
		typename T,
		typename D = std::decay_t<T>, // putting it here so we can use it in Initialization list
		std::enable_if_t<!std::is_same_v<std::decay_t<T>, ari::any>>* = nullptr
	> any(T&& d) : mManager{ get_manager<D>() } {
		mStorage = manager_type<D>::create(std::forward<T>(d));
	}

	~any() { reset(); }
	
	
	template <typename T, typename... A>
	T& emplace(A&&... args) {
		reset();
		mManager = get_manager<T>();
		mStorage = manager_type<T>::create(std::forward<A>(args)...);
	}
	
	void reset() noexcept {
		if (!mManager) return;
		
		mManager->destroy(mStorage);
		mManager = nullptr;
	}
	
	void swap(any& other) noexcept {
		std::swap(mStorage, other.mStorage);
		std::swap(mManager, other.mManager);
	}
	
	bool has_value() const noexcept {
		return mManager != nullptr;
	}
	
	const std::type_info& type() const noexcept {
		return has_value()? mManager->type_id() : typeid(void);
	}
	
	any& operator=(const any& rhs) {
		reset();
		mManager = rhs.mManager;
		mStorage = mManager->clone(rhs.mStorage);
		return *this;
	}
	
	any& operator=(any&& rhs) noexcept {
		swap(rhs);
		return *this;
	}

	template <
		typename T,
		typename D = std::decay_t<T>,
		std::enable_if_t<!std::is_same_v<std::decay_t<T>, ari::any>>* = nullptr
	> any& operator=(T&& rhs) {
		reset();
		mManager = get_manager<D>();
		mStorage = manager_type<D>::create(std::forward<T>(rhs));
		return *this;
	}
	
}; // end class any


template <typename T>
const T* any_cast(const any* a) {
	if (!a)	return nullptr;
	if constexpr (!std::is_copy_constructible_v<std::decay_t<T>>) return nullptr;
	if (a->mManager != any::get_manager<std::decay_t<T>>()) return nullptr; 
	
	return a->get<std::decay_t<T>>();
}

template <typename T>
T* any_cast(any* a) {
	if (!a)	return nullptr;
	if constexpr (!std::is_copy_constructible_v<std::decay_t<T>>) return nullptr;
	if (a->mManager != any::get_manager<T>()) return nullptr; 
	
	return a->get<std::decay_t<T>>();
}

template <typename T>
const T any_cast(const any& a) {
	auto p = any_cast<any_cast_t<T>>(&a);
	if (!p) throw std::bad_any_cast{};
	return static_cast<T>(*p);
}

template <typename T>
T any_cast(any& a) {
	auto p = any_cast<any_cast_t<T>>(&a);
	if (!p) throw std::bad_any_cast{};
	return static_cast<T>(*p);
}

template <typename T>
T any_cast(any&& a) {
	auto p = any_cast<any_cast_t<T>>(&a);
	if (!p) throw std::bad_any_cast{};
	return static_cast<T>(std::move(*p));
}


} // end namespace ari
