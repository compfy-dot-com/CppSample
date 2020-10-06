/** 
 * @file
 * 
 * @brief Array Base
 *
 * @details A class for common array functionality.
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
#ifndef ARRAY_BASE_HPP
#define ARRAY_BASE_HPP

#include <array>
#include <stdexcept>

/**
 * @brief Array Base class.
 */
template<size_t NDIM>
class ArrayBase
{
public:

	static_assert(NDIM, "Number of array dimensions must be larger than zero.");

	// Number of dimensions, for convenience.
	constexpr static size_t ndim = NDIM;

	/// This type.
	typedef ArrayBase<NDIM> this_t;
	/// Type of shape container.
	typedef std::array<size_t, NDIM> shape_t;

	// Constructors to be used only by child classes.
	ArrayBase(shape_t shape):
		_size(computeSize(shape)),
		_shape(std::move(shape)),
		_strides(computeStrides(shape))
	{
	}

	/**
	 * @brief Virtual destructor.
	 */
	~ArrayBase()
	{
	}

	/**
	 * @brief Get array size.
	 */
	size_t size() const
	{
		return _size;
	}

	/**
	 * @brief Get array shape.
	 */
	const shape_t& shape() const
	{
		return _shape;
	}

	/**
	 * @brief Get dimension length.
	 */
	template<size_t DIM>
	size_t dim() const
	{
		static_assert(DIM < NDIM,
				"Dimension index cannot be larger than number of dimensions minus one.");

		return _shape[DIM];
	}

	/**
	 * @brief Computes element offset given array indexes.
	 */
	template<typename... IDX>
	size_t computeOffset(IDX... idx) const
	{
		static_assert(sizeof...(idx) == NDIM,
				"Number of array indexes must be equal to number of dimensions.");

		return computeOffset<0>(idx...);
	}

protected:
	size_t _size;
	shape_t _shape;
	shape_t _strides;

private:

	// Offset computation via variadic template.
	template<size_t DIM, typename... IDX>
	size_t computeOffset(size_t first, IDX... idx) const
	{
		return first * _strides[DIM] + computeOffset<DIM + 1>(idx...);
	}

	template<size_t DIM>
	size_t computeOffset(size_t idx) const
	{
		return idx;
	}

	// Compute array size for a shape vector.
	static size_t computeSize(const shape_t &shape)
	{
		size_t size = 1;
		for(size_t dimLen : shape)
			size *= dimLen;
		if(!size)
			throw std::runtime_error("Array size cannot be zero, check array dimensions.");
		return size;
	}

	// Compute strides for accelerated access.
	static shape_t computeStrides(const shape_t &shape)
	{
		shape_t strides;
		strides.back() = 1;
		for(long i = strides.size() - 1; i >= 1; i--)
			strides[i - 1] = strides[i] * shape[i];
		return strides;
	}
};

#endif // ARRAY_BASE_HPP
