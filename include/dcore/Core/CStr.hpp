#pragma once
#include <dcore/Core/Type.hpp>

namespace dstd
{
	/**
	 * @brief Returns the length of a NULL-terminated string.?
	 *
	 * @tparam T type of the character stored in the string.
	 * @param cstr The string pointer
	 * @return The length of the string.
	 */
	template<typename T>
	USize CStrLength(const T *cstr)
	{
		if(cstr == nullptr) return 0;
		for(USize i = 0; cstr; ++cstr, ++i)
			if(*cstr == 0) return i;
		return (USize)-1; // Unreachable.
	}
} // namespace dstd
