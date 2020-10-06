/**
 * @file
 *
 * @brief Array access tests.
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
#include "Sentry.hpp"

#include <cstdlib>

using namespace std;

namespace test
{

//
// Test access performance via optimized subscript operators.
//
void testArrayAccessMethod1(const test_shape_t &shape, float val)
{
	cout << "### Testing array access method 1 (subscript operators)." << endl;
	cout << "Number of dimensions: " << shape.size() << endl;

	BasicArray<float, NUM_TEST_DIM> a(shape);
	cout << "Array size: " << a.size() << endl;

	auto startTime = chrono::high_resolution_clock::now();

	for(size_t t = 0; t < NUM_TEST_ITER; t++)
	{
		// loops are explicit in order to focus on the array access
		for(size_t i0 = 0; i0 < a.dim<0>(); i0++)
		{
			for(size_t i1 = 0; i1 < a.dim<1>(); i1++)
			{
				for(size_t i2 = 0; i2 < a.dim<2>(); i2++)
				{
					for(size_t i3 = 0; i3 < a.dim<3>(); i3++)
					{
						a[i0][i1][i2][i3] = val;
					}
				}
			}
		}
	}

	auto endTime = chrono::high_resolution_clock::now();
	auto durationNanos = chrono::duration<double, nano>(endTime - startTime).count();
    cout << "Method 1 write time: " << durationNanos/(NUM_TEST_ITER * a.size()) << " ns." << endl;
}

//
// Test access performance via optimized variadic function template.
//
void testArrayAccessMethod2(const test_shape_t &shape, float val)
{
	cout << "### Testing array access method 2 (variadic function template)." << endl;
	cout << "Number of dimensions: " << shape.size() << endl;

	BasicArray<float, NUM_TEST_DIM> a(shape);
	cout << "Array size: " << a.size() << endl;

	auto startTime = chrono::high_resolution_clock::now();

	for(size_t t = 0; t < NUM_TEST_ITER; t++)
	{
		// loops are explicit in order to focus on the array access
		for(size_t i0 = 0; i0 < a.dim<0>(); i0++)
		{
			for(size_t i1 = 0; i1 < a.dim<1>(); i1++)
			{
				for(size_t i2 = 0; i2 < a.dim<2>(); i2++)
				{
					for(size_t i3 = 0; i3 < a.dim<3>(); i3++)
					{
						a(i0, i1, i2, i3) = val;
					}
				}
			}
		}
	}

	auto endTime = chrono::high_resolution_clock::now();
	auto durationNanos = chrono::duration<double, nano>(endTime - startTime).count();
    cout << "Method 2 write time: " << durationNanos/(NUM_TEST_ITER * a.size()) << " ns." << endl;
}

// Examples of array view code.
void demoBasicArrayView()
{
	// turn any buffer into a multi-array
	{
		vector<long> vec(20);

		BasicArrayView<long, 3> vecView(vec.data(), {2, 5, 2}, vec.size());

		// access with functor: notice how convenient (compared to loops)
		vecView.traverse([](const auto &idx, long &data){
			data = idx[0] * 100 + idx[1] * 10 + idx[2];
		});

		cout << "A small 3D array:" << endl;

		// check correctness while accessing with subscript operator
		for(size_t i0 = 0; i0 < vecView.dim<0>(); i0++)
		{
			for(size_t i1 = 0; i1 < vecView.dim<1>(); i1++)
			{
				for(size_t i2 = 0; i2 < vecView.dim<2>(); i2++)
				{
					if(vecView[i0][i1][i2] == static_cast<long>(i0 * 100 + i1 * 10 + i2))
						cout << vecView[i0][i1][i2] << ' ';
					else
						cout << "W(" << vecView[i0][i1][i2] << ") ";
				}
				cout << endl;
			}
			cout << endl;
		}
	}
	// for old C (to demo Sentry as well)
	{
		constexpr size_t BUF_SIZE = 100;

	    int *buf = (int*) malloc(BUF_SIZE * sizeof(int));
	    if(!buf) throw runtime_error("Failed to allocate test buffer.");
		util::Sentry bufSentry([&buf]{ free(buf); });

		BasicArrayView<int, 2> v1(buf, {5, 20}, BUF_SIZE);

		v1.traverse([](const auto &idx, int &data){
			data = idx[0] * 10 + idx[1];
		});

		// Copy and compare
		BasicArray<int, 2> a1(v1.shape());
		a1 << v1;

		if(a1 == v1)
			cout << "Good copy." << endl;
		else
			cout << "Bad copy." << endl;

		// Copy to a different type and shape
		BasicArray<float, 3> a2({10, 5, 2});
		a2 << v1;

		// Demonstrate cloning.
		auto a2Clone = a2.cloneT();

		if(a2Clone->equalValue(v1))
			cout << "Good clone." << endl;
		else
			cout << "Bad clone." << endl;

	}
}

}
