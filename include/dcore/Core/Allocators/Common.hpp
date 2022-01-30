#pragma once
#include <dcore/Core/Type.hpp>
#include <cstdlib>

// TODO: Create a small GP allocator (use tinymalloc or something)

namespace dstd
{
	class CommonAllocator
	{
	public:
		template<typename T>
		static T *Alloc()
		{
			return reinterpret_cast<T *>(std::malloc(sizeof(T)));
		}

		template<typename T>
		static T *AllocN(USize count)
		{
			return reinterpret_cast<T *>(std::malloc(sizeof(T) * count));
		}

		template<typename T>
		static T *ReAllocN(const T *ptr, USize newCount)
		{
			return reinterpret_cast<T *>(std::realloc(const_cast<T *>(ptr), sizeof(T) * newCount));
		}

		template<typename T>
		static void DeAlloc(const T *ptr)
		{
			return std::free(const_cast<T *>(ptr));
		}
	};
} // namespace dstd
