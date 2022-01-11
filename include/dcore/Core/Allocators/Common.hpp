#pragma once
#include <cstdlib>

namespace dstd
{
	class CommonAllocator
	{
	public:
		template<typename T>
		static T *Alloc()
		{
			return reinterpret_cast<T *>(malloc(sizeof(T)));
		}

		template<typename T>
		static T *AllocN(USize count)
		{
			return reinterpret_cast<T *>(malloc(sizeof(T) * count));
		}

		template<typename T>
		static T *ReAllocN(const T *ptr, USize newCount)
		{
			return reinterpret_cast<T *>(realloc(const_cast<T *>(ptr), sizeof(T) * newCount));
		}

		template<typename T>
		static void DeAlloc(const T *ptr)
		{
			return free(const_cast<T *>(ptr));
		}
	};
} // namespace dstd
