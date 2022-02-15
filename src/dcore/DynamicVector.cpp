#include <dcore/Core/DynamicVector.hpp>
#include <dcore/Core/Memory.hpp>
#include <dcore/Core/Allocators/Common.hpp>
#include <dcore/Core/Log.hpp>

namespace dstd
{
	DynamicVector::~DynamicVector()
	{
		CommonAllocator::DeAlloc(Data_);
		Size_ = 0;
		Data_ = nullptr;
	}

	void DynamicVector::RawAdd(const void *value)
	{
		LOG_F(INFO, "DynamicVector::RawAdd()");
		Data_ = CommonAllocator::ReAllocN((Byte *)Data_, (Size_ + 1) * ItemSize_);
		CopyBuffer(ItemSize_, (Byte *)(Data_) + Size_ * ItemSize_, (Byte *)value);
		Size_ += 1;
	}

	void *DynamicVector::RawGet(USize idx) const
	{
		LOG_F(INFO, "DynamicVector::RawGet(%ld)", idx);
		return (Byte *)Data_ + idx * ItemSize_;
	}
} // namespace dstd
