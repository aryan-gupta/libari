


#ifndef ARI_TYPE_TRAITS
#define ARI_TYPE_TRAITS

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


// Enable If
template <bool B, typename T = void>
struct enable_if {};
 
template <typename T>
struct enable_if<true, T> { using type = T; };
	
template <bool B, typename T = void> using enable_if_t = typename enable_if<B,T>::type;


// Is Same
template<typename T, typename U>
struct is_same : false_type {};
 
template<typename T>
struct is_same<T, T> : true_type {};

template <typename T, typename U> constexpr bool is_same_v = is_same<T, U>::value;


// Conditional
template <bool B, typename T, typename U>
struct conditional {};

template <typename T, typename U>
struct conditional<true> { using type = T; };

template <typename T, typename U>
struct conditional<false> { using type = U; };

template <typename B, typename T, typename U>
using conditional_t = typename conditional<B, T, U>::type;

	
} // end namespace ari

#endif // ARI_TYPE_TRAITS defined