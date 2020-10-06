/**
 * @file
 *
 * @brief Array access test templates.
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
#ifndef TEST_ARRAY_HPP
#define TEST_ARRAY_HPP

#include "BasicArray.hpp"

#include <chrono>
#include <iostream>

/// Test namespace.
namespace test
{

/// Number of test iterations.
constexpr size_t NUM_TEST_ITER = 100;

/// Number of test dimensions.
constexpr size_t NUM_TEST_DIM = 4;

typedef std::array<size_t, NUM_TEST_DIM> test_shape_t;

/**
 * @brief Test access performance via optimized subscript operators.
 */
void testArrayAccessMethod1(const test_shape_t &shape, float val);

/**
 * @brief Test access performance via optimized variadic function template.
 */
void testArrayAccessMethod2(const test_shape_t &shape, float val);

/**
 * @brief Test access performance via an index traversing functor.
 *
 * This method of access is most flexible and versatile.
 */
template<typename T, size_t NDIM>
void testArrayAccessMethod3(const std::array<size_t, NDIM> &shape, T val)
{
	using namespace std;

	cout << "### Testing array access method 3 (index visitor functor)." << endl;
	cout << "Number of dimensions: " << shape.size() << endl;

	BasicArray<T, NDIM> a(shape);
	cout << "Array size: " << a.size() << endl;

	auto startTime = chrono::high_resolution_clock::now();

	for(size_t t = 0; t < NUM_TEST_ITER; t++)
	{
		a.traverse([val](const auto &idx, T &data)
		{
			data = val;
		});
	}

	auto endTime = chrono::high_resolution_clock::now();
	auto durationNanos = chrono::duration<double, nano>(endTime - startTime).count();
    cout << "Method 3 write time: " << durationNanos/(NUM_TEST_ITER * a.size()) << " ns." << endl;
}

/**
 * @brief Examples of array view code.
 */
void demoBasicArrayView();

}

#endif // TEST_ARRAY_HPP
