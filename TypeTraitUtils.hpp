/**
 * @file
 *
 * @brief Utilities for type traits.
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
#ifndef TYPE_TRAIT_UTILS_HPP
#define TYPE_TRAIT_UTILS_HPP

#include "NumberTraits.hpp"

namespace util
{

/**
 * @brief Equality comparison for trivial cases: same type.
 *
 * @details Demonstrate use of SFINAE.
 *          One could have also used class template specializations of "if constexpr".
 */
template<typename T>
inline bool
eq(const T &v1, const T &v2)
{
	return v1 == v2;
}

/**
 * @brief Equality comparison for trivial cases: different types but not all integers.
 *
 * @details Demonstrate use of SFINAE.
 *          One could have also used class template specializations of "if constexpr".
 */
template<typename T1, typename T2>
inline std::enable_if_t<!all_integer<T1, T2>, bool>
eq(const T1 &v1, const T2 &v2)
{
	return v1 == v2;
}

/**
 * @brief Equality comparison for integer numbers.
 *
 * @details Comparing signed and unsigned integers requires special care.
 *          Demonstrate use of SFINAE.
 *          One could have also used class template specializations of "if constexpr".
 */
template<typename T1, typename T2>
inline std::enable_if_t<all_integer<T1, T2>, bool>
eq(const T1 &v1, const T2 &v2)
{
	return eq_int<T1, T2>(v1, v2);
}

}

#endif // TYPE_TRAIT_UTILS_HPP

