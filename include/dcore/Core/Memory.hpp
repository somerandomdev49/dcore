#pragma once
#include <dcore/Core/Type.hpp>

namespace dstd
{
	template<typename T, typename U>
	void CopyBuffer(USize bufSize, T *dest, const U *source)
	{
		for(USize i = 0; i != bufSize; ++i) dest[i] = source[i];
	}
} // namespace dstd
