#include <dcore/Core/Memory.hpp>

namespace dstd
{
	Byte *AllocBuffer(USize size)
	{
		return GenericAllocBuffer<CommonAllocator>(size);
	}
} 
