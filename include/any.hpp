
#include <exception>
#include <memory>
#include <typeinfo>
#include <type_traits>
#include <any> // for std::bad_any_cast

namespace ari {
	
using std::bad_any_cast;

/// @brief A class that can hold one of any type of data
class any {
	/// @brief A list of the friend classes, used so we can cast it to the proper type
	template <typename T> friend T* any_cast(any* a);
	template <typename T> friend const T* any_cast(const any* a);
	template <typename T> friend const T any_cast(const any& a);
	template <typename T> friend T any_cast(any& a);
	template <typename T> friend T any_cast(any&& a);
	
	using heap_storage = void*;
	using stack_storage = std::aligned_storage<sizeof(void*), std::alignment_of<void*>::value>::type;
	
	union AnyStorage {
		heap_storage  mPtr;
		stack_storage mBuf;
	};
	
	struct AnyManager {
		// void (*create) (AnyStorage&);
		void* (*get) (AnyStorage&);
		const std::type_info& (*type_id) ();
		AnyStorage (*clone) (const AnyStorage&);
		void (*destroy) (AnyStorage&);
		void (*move) (AnyStorage&, AnyStorage&);
		void (*swap) (AnyStorage& a, AnyStorage& b);
	};
	
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
	
	template <typename T>
	constexpr static bool requires_allocation() {
		return sizeof(T) >= sizeof(AnyStorage) and alignof(T) >= alignof(AnyStorage);
	}
	
	template <typename T, bool b = requires_allocation<T>()> 
	using manager_type = std::conditional_t<b, AnyManagerHeap<std::decay_t<T>>, AnyManagerStack<std::decay_t<T>>>;
	
	template <typename T>
	static AnyManager* get_manager() {
		using F = manager_type<T>;
		
		static AnyManager manager { &F::get, &F::type_id, &F::clone, &F::destroy, &F::move/*, &F::swap */};
		
		return &manager;
	}
	
	template <typename T>
	T* get() {
		return static_cast<T*>(mManager->get(mStorage));
	}
	
	// template <typename T>
	// const T* get() const {
		// return static_cast<const T*>(mManager->get(mStorage));
	// }
	
	AnyManager* mManager;
	AnyStorage  mStorage;
	
public:
	/// @brief Creates an empty any 
	constexpr any() noexcept : mManager{ nullptr }, mStorage{ nullptr } {}
	
	/// @brief Copies constructs an another any from \param other
	/// @param other The any type to copy
	any(const any& other) : mManager{ other.mManager }, mStorage{ mManager->clone(other.mStorage) } {}
	
	any(any&& other) noexcept : mManager { other.mManager } {
		mManager->move(other.mStorage, this->mStorage);
		other.mManager = nullptr;
	}
	
	template <
		typename T,
		typename D = std::decay_t<T>,
		std::enable_if_t<!std::is_same_v<std::decay_t<T>, ari::any>>* = nullptr
	> any(T&& d) : mManager{ get_manager<D>() } {
		mStorage = manager_type<D>::create(d);
	}

	~any() { reset(); }
	
	/// @todo fix this to represent the new functional changes
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

template <typename T> using any_cast_t = std::remove_cv_t<std::remove_reference_t<T>>;

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
	
	// std::cout << a->mManager << "\n" << any::get_manager<std::decay_t<T>>() << "\n";
	
	if (a->mManager != any::get_manager<T>()) return nullptr; 
	// std::cout << "ANYCAST 1\n";
	
	return a->get<std::decay_t<T>>();
}

template <typename T>
const T any_cast(const any& a) {
	// See http://en.cppreference.com/w/cpp/utility/any/any_cast
	// for why we did any_cast_t<T>
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
