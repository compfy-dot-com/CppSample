/**
 * @file
 *
 * @brief Number traits.
 *
 * @details
 *
 * @authors
 * - Alex Ken
 *
 * @version
 * - 10/5/2020 Initial version.
 *
 * @copyright Alexander Ken
 *
 * @par License: The MIT License (MIT)
 */
#ifndef NUMBER_TRAITS_HPP
#define NUMBER_TRAITS_HPP

#include <type_traits>

namespace util
{

/**
 * @ Check if types are integer numbers (using a variadic template variable).
 */
template<typename... T>
constexpr bool all_integer = std::conjunction_v<std::is_integral<T>...> &&
                             !std::disjunction_v<std::is_same<T, bool>...>;

/**
 * @ Check if all signed.
 */
template<typename... T>
constexpr bool all_signed = std::conjunction_v<std::is_signed<T>...>;

/**
 * @ Check if all unsigned.
 */
template<typename... T>
constexpr bool all_unsigned = std::conjunction_v<std::is_unsigned<T>...>;

/**
 * @ Check if types have the same signedness.
 */
template<typename... T>
constexpr bool same_sign = all_signed<T...> || all_unsigned<T...>;

/**
 * @brief Equality comparison for mixed integer numbers.
 *
 * @details Demonstrate use of SFINAE.
 *          One could have also used class template specializations of "if constexpr".
 */
template<typename T1, typename T2>
inline std::enable_if_t<all_integer<T1, T2> && std::is_signed_v<T1> && std::is_unsigned_v<T2>, bool>
eq_int(const T1 v1, const T2 v2)
{
	return v1 >= 0 && static_cast<size_t>(v1) == v2;
}

/**
 * @brief Equality comparison for mixed integer numbers.
 *
 * @details Demonstrate use of SFINAE.
 *          One could have also used class template specializations of "if constexpr".
 */
template<typename T1, typename T2>
inline std::enable_if_t<all_integer<T1, T2> && std::is_unsigned_v<T1> && std::is_signed_v<T2>, bool>
eq_int(const T1 v1, const T2 v2)
{
	return v2 >= 0 && v1 == static_cast<size_t>(v2);
}

/**
 * @brief Equality comparison for integer numbers with same signedness.
 *
 * @details Demonstrate use of SFINAE.
 *          One could have also used class template specializations of "if constexpr".
 */
template<typename T1, typename T2>
inline std::enable_if_t<all_integer<T1, T2> && same_sign<T1, T2>, bool>
eq_int(const T1 v1, const T2 v2)
{
	return v1 == v2;
}

}

#endif // NUMBER_TRAITS_HPP

