/** 
 * @file
 * 
 * @brief Basic Array Traversal
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
#ifndef BASIC_ARRAY_TRAVERSAL_HPP
#define BASIC_ARRAY_TRAVERSAL_HPP

#include <array>

/**
 * @brief Class for traversing multidimensional arrays.
 *
 * @details This traversal supports strided arrays.
 */
template<typename ITER, size_t NDIM>
class BasicArrayTraversal
{
public:
	BasicArrayTraversal(ITER data,
						const std::array<size_t, NDIM> &start,
						const std::array<size_t, NDIM> &end,
						const std::array<size_t, NDIM> &strides):
		_data(data),
		_start(start),
		_end(end),
		_strides(strides),
		_lastDim(NDIM - 1)
	{
	}

	/**
	 * @brief Traverse array and apply functor.
	 */
	template<typename FUN>
	void traverse(FUN &&fun)
	{
		// Supporting zero sized arrays.
		if(_lastDim < 0)
			return;

		std::array<size_t, NDIM> idx{0};

		iterate(_data, 0, idx, std::forward<FUN>(fun));
	}

private:
	ITER _data;
	const std::array<size_t, NDIM> &_start;
	const std::array<size_t, NDIM> &_end;
	const std::array<size_t, NDIM> &_strides;
	const long _lastDim; // Takes value -1 for a zero-sized array.

	template<typename FUN>
	void iterate(ITER iter, size_t dim, std::array<size_t, NDIM> &idx, FUN &&fun)
	{
		const size_t start = _start[dim];
		const size_t end = _end[dim];
		const size_t stride = _strides[dim];
		size_t &i = idx[dim];

		// last dimension to iterate
		if(dim == static_cast<size_t>(_lastDim))
		{
			for(i = start; i < end; i++, iter += stride)
				fun(const_cast<const std::array<size_t, NDIM>&>(idx), *iter);
		}
		// continue iteration
		else
		{
			const size_t nextDim = dim + 1;

			for(i = start; i < end; i++, iter += stride)
				iterate(iter, nextDim, idx, std::forward<FUN>(fun));
		}
	}
};

#endif // BASIC_ARRAY_TRAVERSAL_HPP
