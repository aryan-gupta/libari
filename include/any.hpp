
#include <exception>
#include <memory>
#include <typeinfo>
#include <type_traits>
#include <any> // for std::bad_any_cast

namespace ari {


/// @brief A class that can hold one of any type of data
class any {
	using heap_pointer = void*;
	using stack_storage = aligned_storage<sizeof(mStorage), alignof(void*)>::type;
	
	// if you are those people that read whats in the system headers, you will realize that
	// this is exactally how GCC implements std::any
	enum class OP { ACCESS, TYPEID, CLONE, DESTROY, MOVE };
	union ManageArg {
		heap_pointer mObj;
		const std::type_info* mType;
		any* mAny;
	};
	
	/// Private members
	void (*mManage) (Op oper, any& a, ManageArg& m); // function pointer -- return void pointer
	union {
		heap_pointer mPointer;
		stack_storage mStorage;
	} 
	
	// This 'function' alows us to choose between whether we should use SIO or an heap object
	// if our type T fits into our union, then we want to use stack (use_stack is true)
	template <
		typename T,
		bool F = (sizeof(T) < sizeof(heap_pointer)) and (alignof(T) <= alignof(heap_pointer))
	> using use_stack = std::integral_constant<bool, F>;
	
public:
	/// @brief A list of the friend classes, used so we can cast it to the proper type
	template <typename T> friend T* any_cast(any* a);
	template <typename T> friend const T* any_cast(const any* a);
	template <typename T> friend const T any_cast(const any& a);
	template <typename T> friend T any_cast(any& a);
	template <typename T> friend T any_cast(any&& a);
	
	/// @brief Creates an empty any 
	constexpr any() noexcept : mManage{}, mStorage{} {}
	
	
	/// @brief Copies constructs a another any
	/// @param other The any type to copy
	any(const any& other) : mManage{ other.mManage }, mStorage{} {
		if (!mManage) return;
		
		ManageArg arg;
		arg.mAny = this;
		mManage(OP::CLONE, other, arg);
	}
	
	
	any(any&& other) noexcept : mManage { other.mManage } {
		if (!mManage) return;
		
		ManageArg arg;
		
	}
	
	
	/**
		This function firsts decays T, this is mostly used if we want to 
		store a static array. It will store the actual array, however,
		the function will store the decay type information
	*/
	template <
		typename T,
		typename D = std::decay_t<T>,
		std::enable_if_t<!std::is_same_v<std::decay_t<T>, ari::any>>* = nullptr
	> any(T&& d) : mStorage{ new data<T>(std::forward<T>(d)) }, mManage{ &<D> } {}

	~any() { reset(); }
	
	/// @todo fix this to represent the new functional changes
	template <typename T, typename... A>
	T& emplace(A&&... args) {
		reset();
		mStorage = new data<T>(std::forward<A>(args)...);
	}
	
	void reset() noexcept {
		if (!mManage) return;
		
		delete mStorage;
		mManage = nullptr;
		mStorage = nullptr;
	}
	
	void swap(any& other) noexcept {
		std::swap(mStorage, other.mStorage);
		std::swap(mManage, other.mManage);
	}
	
	bool has_value() const noexcept {
		return mManage != nullptr;
	}
	
	const std::type_info& type() const noexcept {
		return has_value()? mStorage->type_id() : typeid(void);
	}
	
	any& operator=(const any& rhs) {
		reset();
		mStorage = rhs.mStorage->clone();
		mManage = rhs.mManage;
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
		mStorage = new data<T>(std::forward<T>(rhs));
		mManage = &get_pointer<D>;
		return *this;
	}
	
private:	
	// I was really hoping that I didn't have to copy other people's code
	// but the GCC implementation is soooo good
	
	/// After looking through GCC's implementation of std::any, they use a
	/// cool approch that I will try to emplain below. 
	/**
		The private member of std::any is the actual storage, a union of a
		void pointer and a arbitrary storage same size of the void pointer. 
		This makes this the overall size of the storage just sizeof(void*)
		The other member data is function pointer to a 'manager' this manager
		does most of the hardwork. There are 2 types of managers that they 
		implement, one for the void* (heap item) and one for aligned storage
		(stack item). The pointer changes depending on what item is stored
		this way when you do an any_cast, it checks if the address of the 
		actual function of the type is equal to the private member of 
		std::any. 
	*/
	/// @brief get function for the pointer.
	/**
		@todo when we implement small item optimization, we will another
		function for it
	*/
	template <typename T>
	static void manage_pointer(Op oper, any& a, ManageArg& m) {
		auto ptr = static_cast<T*>(a.mPointer);
		switch (oper) {
			case ACCESS: // returns the data from \param a storing it in \param m
				m.mObj = ptr;
			return;
			
			case TYPEID: // returns the typeid of \param a and stores it in \param m
				// This feels really wrong taking the addess of an expiring value, need to research
				m.mType = &typeid(T);
			return;
			
			case CLONE: // clone \param a into \param m (source, dest)
				m.mAny->mPointer = new T{*ptr};
				m.mAny->mManage = a.mManage;
			return;
			
			case DESTROY: // destroys \param a
				delete ptr;
			return;
			
			case MOVE: // moves \param a to \param m
				m.mAny->mPointer = ptr;
				m.mAny->mManage = a.mManage;
				a = any{};
			return;
		}
	}
	
	template <typename T>
	static void manage_small_item_opt(Op oper, any& a, ManageArg& m) {
		auto ptr = reinterpret_cast<T*>(&a.mStorage);
		switch (oper) {
			case ACCESS:
				m.mObj = ptr;
			return;
			
			case TYPEID:
				// This feels really wrong taking the addess of an expiring value, need to research
				m.mType = &typeid(T);
			return;
			
			case CLONE:
				m.mAny->mPointer = new T{*ptr};
				m.mAny->mManage = a.mManage;
			return;
			
			case DESTROY:
				ptr->~T(); // call destructor
			return;
			
			case MOVE:
				
			return;
		}
	}
	
	template <typename T>
	static void create_small_item_opt()
	
}; // end class any

template <typename T> using any_cast_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T>
const T* any_cast(const any* a) {
	using D = std::decay_t<T>;
	
	if (!a)	return nullptr;
	if constexpr (!std::is_copy_constructible_v<std::decay_t<T>>) return nullptr;
	if (a->mManage != &any::get_pointer<D>) return nullptr; 
	
	// Ok alot is happening on this line. First we take the ari::any private pointer (any_base*)
	// and we cast it to a derived class any_data<T> pointer.
	auto p = static_cast<any_data<T>*>(a->mStorage);
	
	// Then we cast it to the proper type
	// Note that becsue this returns a pointer, it takes care of the static array 
	// issue we had in the earlier version
	return static_cast<T*>(&p->mData);
}

template <typename T>
const T any_cast(const any& a) {
	// See http://en.cppreference.com/w/cpp/utility/any/any_cast
	// for why we did any_cast_t<T>
	auto p = any_cast<any_cast_t<T>>(&a);
	if (!p) throw std::bad_any_cast{};
	return static_cast<T>(*p);
}

/// The below 3 functions are just non-const and r-value variations of the top 2 functions

template <typename T>
T* any_cast(any* a) {
	using D = std::decay_t<T>;
	
	if (!a)	return nullptr;
	if constexpr (!std::is_copy_constructible_v<std::decay_t<T>>) return nullptr;
	if (a->mManage != &any::get_pointer<D>) return nullptr; 
	
	auto p = static_cast<any_data<T>*>(a->mStorage);
	return static_cast<T*>(&p->mData);
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
