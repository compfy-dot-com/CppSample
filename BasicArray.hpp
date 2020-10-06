/** 
 * @file
 * 
 * @brief Basic (contiguous) array.
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
#ifndef BASIC_ARRAY_HPP
#define BASIC_ARRAY_HPP

#include "BasicArrayView.hpp"
#include "ClonableBase.hpp"

#include <vector>

/**
 * @brief Basic (contiguous) array.
 *
 * @details A contiguous array container and functionality.
 *          Clonable.
 */
template<typename T, size_t NDIM>
class BasicArray final:
	public BasicArrayView<T, NDIM>,
	public ClonableBase<BasicArray<T, NDIM>>
{
public:

	/// This type.
	typedef BasicArray<T, NDIM> this_t;
	/// Base type.
	typedef BasicArrayView<T, NDIM> base_t;
	/// Type of shape container.
	typedef typename base_t::shape_t shape_t;

	/**
	 * @brief Constructors which initializes with default value.
	 */
	BasicArray(shape_t shape):
		base_t(std::move(shape)),
		_container(this->size())
	{
		this->_data = _container.data();
	}

	/**
	 * @brief Constructors which initializes with provided value.
	 */
	BasicArray(shape_t shape, const T &value):
		base_t(std::move(shape)),
		_container(this->size(), value)
	{
		this->_data = _container.data();
	}

private:
	std::vector<T> _container;

};

#endif // BASIC_ARRAY_HPP
