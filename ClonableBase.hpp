/**
 * @file
 *
 * @brief Clonable interface.
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
#ifndef CLONABLE_BASE_HPP
#define CLONABLE_BASE_HPP

#include "Clonable.hpp"

/**
 * @brief Provides a standard clonable implementation using CRTP.
 *
 * Demonstrates both the runtime and the compile time polymophism.
 */
template<typename CHILD>
struct ClonableBase: public Clonable
{
	/**
	 * @brief Virtual function implementation via CRTP.
	 */
	std::shared_ptr<Clonable> clone() const override
	{
		return std::make_shared<CHILD>(static_cast<const CHILD&>(*this));
	}

	/**
	 * @brief Inheritable clone cast to the desired type.
	 */
	std::shared_ptr<CHILD> cloneT() const
	{
		return std::static_pointer_cast<CHILD>(clone());
	}
};

#endif // CLONABLE_BASE_HPP

