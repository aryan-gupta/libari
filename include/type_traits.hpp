


#ifndef ARI_TYPE_TRAITS
#define ARI_TYPE_TRAITS

// Please note that 80% of these I did not write myself. However, I copied with understanding of
// how these type manipulations work

namespace ari {

// Integral Constant
template <typename T, T val>
struct integral_constant {
    using value_type = T;
    typedef integral_constant type;
	
	static constexpr value_type value = val;
	
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

template <bool B> using bool_constant = integral_constant<bool, B>;
using true_type  = bool_constant<true>;
using false_type = bool_constant<false>;


// READ : // https://stackoverflow.com/questions/5625600 for ... ... info
template <typename R> struct is_function : false_type { };
// Ya, get your eyes ready
template <typename R, typename... A> struct is_function<R(A...)> : true_type {};
template <typename R, typename... A> struct is_function<R(A......)> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) const> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) volatile> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) const volatile> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) const> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) volatile> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) const volatile> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) &> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) const &> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) volatile &> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) const volatile &> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) &> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) const &> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) volatile &> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) const volatile &> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) &&> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) const &&> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) volatile &&> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) const volatile &&> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) &&> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) const &&> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) volatile &&> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) const volatile &&> : true_type {}; 
template <typename R, typename... A> struct is_function<R(A...) noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) const noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) volatile noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) const volatile noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) const noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) volatile noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) const volatile noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) & noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) const & noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) volatile & noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) const volatile & noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) & noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) const & noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) volatile & noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) const volatile & noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) && noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) const && noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) volatile && noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A...) const volatile && noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) && noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) const && noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) volatile && noexcept> : true_type {};
template <typename R, typename... A> struct is_function<R(A......) const volatile && noexcept> : true_type {};


// Enable If
template <bool B, typename T = void> struct enable_if {};
template <typename T> struct enable_if<true, T> { using type = T; };
template <bool B, typename T = void> using enable_if_t = typename enable_if<B,T>::type;


// Is Same
template<typename T, typename U> struct is_same : false_type {};
template<typename T> struct is_same<T, T> : true_type {};
template <typename T, typename U> constexpr bool is_same_v = is_same<T, U>::value;


// Conditional
template <bool B, typename T, typename U> struct conditional {};
template <typename T, typename U> struct conditional<true> { using type = T; };
template <typename T, typename U> struct conditional<false> { using type = U; };
template <bool B, typename T, typename U> using conditional_t = typename conditional<B, T, U>::type;


// Is Array
template <typename T> struct is_array : false_type {};
template <typename T> struct is_array<T[]> : true_type {};
template <typename T, size_t N> struct is_array<T[N]> : true_type {};


// Remove Extent
template <typename T> struct remove_extent { using type = T; };
template <typename T> struct remove_extent<T[]> { using type = T; };
template <typename T, size_t N> struct remove_extent<T[N]> { using type = T; };
template <typename T> using remove_extent_t = typename remove_extent<T>::type;


// Remove Reference
template <typename T> struct remove_reference { using type = T; };
template <typename T> struct remove_reference<T&> { using type = T; };
template <typename T> struct remove_reference<T&&> { using type = T; };
template <typename T> using remove_reference_t = typename remove_reference<T>::type;


// Add Pointer 
template <typename T>
struct add_pointer { using type = conditional_t<is_function_t<T>, T, remove_reference_t<T>*>; };

template< typename T, typename... A> struct add_pointer<T(A...)> { using type = T(*)(A...); };

template< typename T, typename... A> 
struct add_pointer<T(A......)> { using type = T(*)(A......); };

template <typename T> using add_pointer_t = typename add_pointer<T>::type;


// Remove Const Volitile
template <typename T> struct remove_const { using type = T; };
template <typename T> struct remove_const<const T> { using type = T; };
template <typename T> using remove_const_t = typename remove_const<T>::type;
 
template <typename T> struct remove_volatile { using type = T; };
template <typename T> struct remove_volatile<volatile T> { using type = T; };
template <typename T> using remove_volatile_t = typename remove_volatile<T>::type;

template <typename T>
struct remove_cv { using type = std::remove_volatile_t<std::remove_const_t<T>>; };
template <typename T> using remove_cv_t = typename remove_cv<T>::type;


// Decay
template <typename T>
struct decay {
	using type = conditional_t<is_array_t<U>, remove_extent_t<U>*,
		conditional_t<is_function_t<U>, add_pointer_t<U>, remove_cv_t<U>>>;
};

	
} // end namespace ari

#endif // ARI_TYPE_TRAITS defined