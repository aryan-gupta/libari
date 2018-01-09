
#include <exception>
#include <memory>
#include <typeinfo>
#include <type_traits>
#include <any> // for std::bad_any_cast

namespace ari {
// namespace ari is a namespace I have been
// using for alot of my code where I implement
// my 'own' std:: stuctures and algorithms


struct any_base {
	virtual ~any_base() = default;
	virtual any_base* clone() const = 0;
	virtual const std::type_info& type_id() const = 0;
};


template <typename T>
struct any_data : public any_base {
	using value_type = T;
	
	any_data(const value_type& vt) : mData{ vt } {}
	
	virtual ~any_data() = default;
	virtual const std::type_info& type_id() const { return typeid(value_type); }
	virtual any_data* clone() const { return new any_data{ mData }; }
	
	value_type mData;
};


/// @brief A class that can hold one of any type of data
/**
	A lot of this implementation was used from
	http://www.two-sdg.demon.co.uk/curbralan/papers/ValuedConversions.pdf
	This is a good read if you want to better understand what's going on here
*/

class any {
public:
	/// @brief A list of the friend classes, used so we can cast it to the proper type
	template <typename T> friend T* any_cast(any* a);
	template <typename T> friend const T* any_cast(const any* a);
	template <typename T> friend const T any_cast(const any& a);
	template <typename T> friend T any_cast(any& a);
	template <typename T> friend T any_cast(any&& a);
	
	/// @brief Creates an empty any 
	constexpr any() noexcept : mStorage{ nullptr }, mGet{ nullptr } {}
	
	/// @brief Copies constructs a another any
	/// @param other The any type to copy
	any(const any& other) : mStorage{ other.mStorage->clone() }, mGet{ other.mGet } {}
	
	any(any&& other) noexcept : mStorage{ other.mStorage }, mGet { other.mGet } {
		other.mStorage = nullptr;
		other.mGet = nullptr;
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
	> any(T&& d) : mStorage{ new data<T>(std::forward<T>(d)) }, mGet{ &get_pointer<D> } {}

	~any() { reset(); }
	
	/// @todo fix this to represent the new functional changes
	template <typename T, typename... A>
	T& emplace(A&&... args) {
		reset();
		mStorage = new data<T>(std::forward<A>(args)...);
	}
	
	void reset() noexcept {
		if (!mGet) return;
		
		delete mStorage;
		mGet = nullptr;
		mStorage = nullptr;
	}
	
	void swap(any& other) noexcept {
		std::swap(mStorage, other.mStorage);
		std::swap(mGet, other.mGet);
	}
	
	bool has_value() const noexcept {
		return mGet != nullptr;
	}
	
	const std::type_info& type() const noexcept {
		return has_value()? mStorage->type_id() : typeid(void);
	}
	
	any& operator=(const any& rhs) {
		reset();
		mStorage = rhs.mStorage->clone();
		mGet = rhs.mGet;
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
		mGet = &get_pointer<D>;
		return *this;
	}
	
private:	
	using base = any_base;
	template <typename T> using data = any_data<T>;
	
	base* mStorage;
	void* (*mGet) (const any& a); // function pointer -- return void pointer 
	
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
	static void* get_pointer(const any& a) {
		auto p = static_cast<any_data<T>*>(a.mStorage);
		return static_cast<T*>(&p->mData);
	}
	
}; // end class any

template <typename T> using any_cast_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T>
const T* any_cast(const any* a) {
	using D = std::decay_t<T>;
	
	if (!a)	return nullptr;
	// if constexpr (!std::is_copy_constructible_v<std::decay_t<T>>) return nullptr;
	if (a->mGet != &any::get_pointer<D>) return nullptr; 
	
	// Ok alot is happening on this line. First we take the ari::any private pointer (any_base*)
	// and we cast it to a derived class any_data<T> pointer.
	auto p = static_cast<any_data<T>*>(a->mStorage);
	
	// Then we cast it to the proper type
	// Note that becsue this returns a pointer, it takes care of the static array 
	// issue we had in the earlier version
	return static_cast<T*>(&p->mData);
}

template <typename T>
T* any_cast(any* a) {
	using D = std::decay_t<T>;
	
	if (!a)	return nullptr;
	if constexpr (!std::is_copy_constructible_v<std::decay_t<T>>) return nullptr;
	if (a->mGet != &any::get_pointer<D>) return nullptr; 
	
	auto p = static_cast<any_data<T>*>(a->mStorage);
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
