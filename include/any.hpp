
// A lot of this implementation was used from
// http://www.two-sdg.demon.co.uk/curbralan/papers/ValuedConversions.pdf
// This is a good read if you want to better
// understand what's going on here

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

class any {
public:
	template <typename T> friend T any_cast(any& a);

	constexpr any() noexcept : mItem{ nullptr } {}
	
	any(const any& other) : mItem{ other.mItem->clone() } {}
	any(any&& other) noexcept : mItem{ other.mItem } { other.mItem = nullptr; }
	
	template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, ari::any>>* = nullptr>
	any(T&& d) : mItem{ new data<T>(std::forward<T>(d)) } {}

	~any() { reset(); }
	
	template <typename T, typename... A>
	T& emplace(A&&... args) {
		reset();
		mItem = new data<T>(std::forward<A>(args)...);
	}
	
	void reset() noexcept {
		if (!mItem) return;
		
		delete mItem;
		mItem = nullptr;
	}
	
	void swap(any& other) noexcept { std::swap(mItem, other.mItem);	}
	
	bool has_value() const noexcept { return mItem != nullptr; }
	
	const std::type_info& type() const noexcept {
		return has_value()? mItem->type_id() : typeid(void);
	}
	
	any& operator=(const any& rhs) {
		reset();
		mItem = rhs.mItem->clone();
		return *this;
	}
	
	any& operator=(any&& rhs) noexcept {
		swap(rhs);
		return *this;
	}

	template<typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, ari::any>>* = nullptr>
    any& operator=(T&& rhs) {
		reset();
		mItem = new data<T>(std::forward<T>(rhs));
		return *this;
	}
	
private:	
	using base = any_base;
	template <typename T> using data = any_data<T>;
	
	base* mItem;
	
}; // end class any

template <typename T>
T any_cast(any& a) {
	if (a.mItem->type_id() != typeid(T))
		throw std::bad_any_cast{};
	// Ok alot is happening on this line. First we take the ari::any data and we cast
	// it to the derived class any_data<T> pointer. Then we can safely acces the internal
	// data that the ari::any class is holding. 
	return static_cast<any_data<T>*>(a.mItem)->mData;
}


} // end namespace ari