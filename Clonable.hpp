/**
 * @file
 *
 * @brief Clonable interface (abstract class)
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
#ifndef CLONABLE_HPP
#define CLONABLE_HPP

#include <memory>

/**
 * @brief Clonable interface.
 */
struct Clonable
{
	/**
	 * @brief Create and return a copy of the object.
	 */
	virtual std::shared_ptr<Clonable> clone() const = 0;

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~Clonable()
	{
	}
};

#endif // CLONABLE_HPP

