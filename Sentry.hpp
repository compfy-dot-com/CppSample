/**
 * @file
 *
 * @brief Sentry pattern.
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
#ifndef SENTRY_HPP
#define SENTRY_HPP

#include <functional>

namespace util
{

class Sentry
{
public:
	typedef std::function<void()> finally_t;

	explicit Sentry(const finally_t &finally):
		_finally(finally)
	{
	}

	~Sentry() noexcept
	{
		try
		{
			_finally();
		}
		catch(...)
		{
		}
	}

private:
	const finally_t _finally;
};

}

#endif // SENTRY_HPP

