/** 
 * @file
 * 
 * @brief Basic (contiguous) Array View
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
#ifndef BASIC_ARRAY_VIEW_HPP
#define BASIC_ARRAY_VIEW_HPP

#include "ArrayBase.hpp"
#include "BasicArrayTraversal.hpp"
#include "TypeTraitUtils.hpp"

/**
 * @brief Basic (contiguous) Array View class.
 */
template<typename T, size_t NDIM>
class BasicArrayView: public ArrayBase<NDIM>
{
public:

	/// This type.
	typedef BasicArrayView<T, NDIM> this_t;
	/// Base type.
	typedef ArrayBase<NDIM> base_t;
	/// Type of shape container.
	typedef typename base_t::shape_t shape_t;
	/// Data type.
	typedef T data_t;
	/// Iterator type.
	typedef T* iterator;
	/// Constant iterator type.
	typedef const T* const_iterator;
	/// Reference type.
	typedef T& reference;
	/// Constant reference type.
	typedef const T& const_reference;

	/**
	 * @brief Public constructors.
	 */
	BasicArrayView(T *data, shape_t shape):
		base_t(std::move(shape)),
		_data(data)
	{
		if(!data)
			throw std::runtime_error("Array view data pointer cannot be null.");
	}

	/**
	 * @brief Public constructors.
	 *
	 * Use parameter total size for extra safety.
	 */
	BasicArrayView(T *data, shape_t shape, size_t totalSize):
		BasicArrayView(data, shape)
	{
		if(!data)
			throw std::runtime_error("Array view data pointer cannot be null.");

		if(this->size() != totalSize)
			throw std::runtime_error("Shape does not match total size.");
	}

	/**
	 * @brief Get begin iterator.
	 */
	iterator begin()
	{
		return _data;
	}

	/**
	 * @brief Get const begin iterator.
	 */
	const_iterator begin() const
	{
		return _data;
	}

	/**
	 * @brief Get end iterator.
	 */
	iterator end()
	{
		return _data + this->size();
	}

	/**
	 * @brief Get const end iterator.
	 */
	const_iterator end() const
	{
		return const_cast<this_t*>(this)->end();
	}

	/**
	 * @brief Access elements of the array via indexes.
	 */
	template<typename... IDX>
	reference operator()(IDX... idx)
	{
		return *(_data + this->computeOffset(idx...));
	}

	/**
	 * @brief Access elements of the constant array via indexes.
	 */
	template<typename... IDX>
	const_reference operator()(IDX... idx) const
	{
		return const_cast<this_t&>(*this)(idx...);
	}

	/**
	 * @brief Subscript operator helper template.
	 *
	 * @details Supports subscript operator.
	 */
	template<typename ITER, typename REF, size_t DIM = 0>
	class Subscript final
	{
	public:
		Subscript<ITER, REF, DIM + 1> operator[](size_t idx)
		{
			const size_t stride = *(_strides++);
			return Subscript<ITER, REF, DIM + 1>(_iter + idx * stride, _strides);
		}

	private:
		ITER _iter;
		const size_t *_strides;

		Subscript(ITER iter, const size_t *strides): _iter(iter), _strides(strides) {}
		Subscript(const Subscript&) = delete;
		Subscript& operator=(const Subscript&) = delete;
		friend class BasicArrayView;
	};

	/**
	 * @brief Subscript operator helper template specialization.
	 */
	template<typename ITER, typename REF>
	class Subscript<ITER, REF, NDIM - 1> final
	{
	public:
		REF operator[](size_t idx)
		{
			return *(_iter + idx);
		}

	private:
		ITER _iter;
		const size_t *_strides;

		Subscript(ITER iter, const size_t *strides): _iter(iter), _strides(strides) {}
		Subscript(const Subscript&) = delete;
		Subscript& operator=(const Subscript&) = delete;
		friend class BasicArrayView;
	};

	/**
	 * @brief Subscript operator.
	 */
	std::conditional_t<NDIM == 1, reference, Subscript<iterator, reference, 1>>
	operator[](size_t idx)
	{
		if constexpr (NDIM == 1)
			return *(_data + idx);
		else
		{
			const size_t *strides = this->_strides.data();
			const size_t stride = *(strides++);
			return Subscript<iterator, reference, 1>(_data + idx * stride, strides);
		}
	}

	/**
	 * @brief Subscript operator.
	 */
	std::conditional_t<NDIM == 1, const_reference, Subscript<const_iterator, const_reference, 1>>
	operator[](size_t idx) const
	{
		if constexpr (NDIM == 1)
			return *(_data + idx);
		else
		{
			const size_t *strides = this->_strides.data();
			const size_t stride = *(strides++);
			return Subscript<const_iterator, const_reference, 1>(_data + idx * stride, strides);
		}
	}

	/**
	 * @brief Traverse array indexes while calling a functor.
	 */
	template<typename FUN>
	void traverse(FUN &&fun)
	{
		BasicArrayTraversal<iterator, NDIM>(this->_data, shape_t{0}, this->_shape, this->_strides).
				traverse(std::forward<FUN>(fun));
	}

	/**
	 * @brief Traverse array indexes while calling a functor.
	 */
	template<typename FUN>
	void traverse(FUN &&fun) const
	{
		BasicArrayTraversal<const_iterator, NDIM>(this->_data, shape_t{0}, this->_shape, this->_strides).
				traverse(std::forward<FUN>(fun));
	}

	/**
	 * @brief Comparison operator
	 */
	bool operator==(const this_t &other) const
	{
		if(this == &other)
			return true;

		if(this->_shape != other._shape)
			return false;

		const_iterator thisIter = _data;
		const_iterator otherIter = other.begin();
		const const_iterator thisIterEnd = end();

		while(thisIter != thisIterEnd && *thisIter++ == *otherIter++);
		return thisIter == thisIterEnd;
	}

	/**
	 * @brief Copy data operator.
	 *
	 * @details
	 *        Allows to copy only the data.
	 *        Reserve normal copy operator for copying the view while pointing to the same data.
	 *
	 * @throws Runtime error of array sizes are unequal.
	 */
	template<typename OT, size_t ONDIM>
	BasicArrayView& operator<<(const BasicArrayView<OT, ONDIM> &other)
	{
		if(reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&other))
			return *this;

		if(this->_size != other.size())
			throw std::runtime_error("Cannot copy data: array sizes do not match.");

		iterator thisIter = _data;
		typename BasicArrayView<OT, ONDIM>::const_iterator otherIter = other.begin();
		const const_iterator thisIterEnd = end();

		while(thisIter != thisIterEnd)
			*thisIter++ = *otherIter++;

		return *this;
	}

	/**
	 * @brief Scientifically motivated comparing of stored values.
	 *
	 * @details Allows scientific number comparison without copying.
	 *          Return false if array sizes differ.
	 */
	template<typename OT, size_t ONDIM>
	bool equalValue(const BasicArrayView<OT, ONDIM> &other) const
	{
		if(reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&other))
			return true;

		if(this->_size != other.size())
			return false;

		iterator thisIter = _data;
		typename BasicArrayView<OT, ONDIM>::const_iterator otherIter = other.begin();
		const const_iterator thisIterEnd = end();

		while(thisIter != thisIterEnd && util::eq(*thisIter++, *otherIter++));
		return thisIter == thisIterEnd;
	}

protected:
	T *_data;

	// Protected constructor.
	BasicArrayView(shape_t shape):
		base_t(std::move(shape)),
		_data(nullptr)
	{
	}
};

#endif // BASIC_ARRAY_VIEW_HPP
