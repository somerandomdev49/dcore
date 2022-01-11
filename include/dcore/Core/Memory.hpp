#pragma once
#include <dcore/Core/Type.hpp>
#include <dcore/Core/Allocators/Common.hpp>

namespace dstd
{
	template<typename T, typename U>
	void CopyBuffer(USize bufSize, T *dest, const U *source)
	{
		for(USize i = 0; i != bufSize; ++i) dest[i] = source[i];
	}

	template<typename Allocator>
	Byte *GenericAllocBuffer(USize size)
	{
		return Allocator::template AllocN<Byte>(size);
	}
	Byte *AllocBuffer(USize size);

} // namespace dstd
