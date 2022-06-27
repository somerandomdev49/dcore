#pragma once
#include <dcore/Core/Type.hpp>
#include <dcore/Core/CStr.hpp>
#include <dcore/Core/Memory.hpp>
#include <dcore/Core/Allocators/Common.hpp>

#ifndef DCORE_DSTD_SHORT_STRING_SIZE
#define DCORE_DSTD_SHORT_STRING_SIZE 32
#endif

namespace dstd
{
	template<typename T, template Allocator>
	class GenericString
	{
	public:
		String() : Size_(0), Data_(nullptr) { Alloc_(); }
		String(const T *cstr) : Data_(nullptr) { CopyFrom(cstr); }

		/**
		 * @brief Copies data from a NULL-terminated string pointer into the internal buffer.
		 *
		 * @param cstr The string pointer
		 */
		void CopyFrom(const T *cstr)
		{
			Size_ = CStrLength(cstr);
			Alloc_();
			CopyBuffer(Size_ + 1, Data_, cstr); // This includes the 0.
		}

		USize Length() const { return Size_ && (Size_ - 1); }

		void Add(T c)
		{
			Size_ += 1;
			ReAlloc_();
			Data_[Size_] = c;
		}

	private:
		void Alloc_()
		{
			// Can we optimize this? Seems to have a lot of ifs (predictable tho)
			if(Size_ == 1)
			{
				if(Data_ != nullptr) DeAlloc_();
				return;
			}

			if(Data_ != nullptr)
			{
				ReAlloc_();
				return;
			}

			if(Size_ > DCORE_DSTD_SHORT_STRING_SIZE) Data_ = Allocator::AllocateN<T>(Size_);
			else if(Data_ == nullptr)
				Data_ = Allocator::AllocateN<T>(DCORE_DSTD_SHORT_STRING_SIZE);
		}

		void ReAlloc_(/* USize oldSize */)
		{
			if(Size_ == 0) DeAlloc_();
			if(Size_ < DCORE_DSTD_SHORT_STRING_SIZE)
				Data_ = Allocator::ReAllocateN<T>(Data_, DCORE_DSTD_SHORT_STRING_SIZE);
			else // TODO: Optimize this
				Data_ = Allocator::ReAllocateN<T>(Data_, Size_);
		}

		//                      Implementation detail:
		// |   Data_   |    Size_   |  Allocated  |   Length()  |  Short?  |
		// |  nullptr  |     0      |      0      |      0      |    --    |
		// |   "\0"    |     1      |    SHORT    |      0      |   True   |
		// |  "...\0"  |   N + 1    |    SHORT    |      N      |   True   |
		// |  "...\0"  |   N + 1    |    N + 1    |      N      |   Fals   |

		T *Data_;
		USize Size_;
	};

	using String  = GenericString<char, CommonAllocator>;
	using WString = GenericString<wchar_t, CommonAllocator>;
} // namespace dstd
