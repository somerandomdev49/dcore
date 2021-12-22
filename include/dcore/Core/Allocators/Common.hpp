#pragma once
#include <cstdlib>

namespace dstd
{
	class CommonAllocator
	{
	public:
		template<typename T>
		static T *Allocate()
		{
			return reinterpret_cast<T *>(malloc(sizeof(T)));
		}

		template<typename T>
		static T *AllocateN(USize count)
		{
			return reinterpret_cast<T *>(malloc(sizeof(T) * count));
		}

		template<typename T>
		static T *ReAllocateN(const T *ptr, USize newCount)
		{
			return reinterpret_cast<T *>(realloc(const_cast<T *>(ptr), sizeof(T) * count));
		}

		template<typename T>
		static void DeAllocate(const T *ptr)
		{
			return free(const_cast<T *>(ptr));
		}
	};
} // namespace dstd
