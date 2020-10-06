/**
 * @file
 *
 * @brief Demo program
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

#include "TestArray.hpp"

using namespace std;
using namespace util;

int main()
{
	// Run demo examples of arrays.
	test::demoBasicArrayView();

	////////// performance tests //////////
	constexpr size_t LEN = 100;

	test::test_shape_t shape{LEN, LEN, LEN, LEN};
	float val = 3.14;

	cout << endl << "Performance testing: number of iterations " <<
			test::NUM_TEST_ITER << '.' << endl;

	// Test access performance via optimized subscript operators.
	test::testArrayAccessMethod1(shape, val);

	// Test access performance via optimized variadic function template.
	test::testArrayAccessMethod2(shape, val);

	// Test access performance via an index traversing functor.
	test::testArrayAccessMethod3(shape, val);

	/* Output:
		A small 3D array:
		0 1
		10 11
		20 21
		30 31
		40 41

		100 101
		110 111
		120 121
		130 131
		140 141

		Good copy.
		Good clone.

		Performance testing: number of iterations 100.
		### Testing array access method 1 (subscript operators).
		Number of dimensions: 4
		Array size: 100000000
		Method 1 write time: 1.02376 ns. (classics highly optimized)
		### Testing array access method 2 (variadic function template).
		Number of dimensions: 4
		Array size: 100000000
		Method 2 write time: 0.94222 ns. (tends to be the faster one)
		### Testing array access method 3 (index visitor functor).
		Number of dimensions: 4
		Array size: 100000000
		Method 3 write time: 1.02869 ns (very cool: flexibility and speed)
	 */

	return 0;
}

