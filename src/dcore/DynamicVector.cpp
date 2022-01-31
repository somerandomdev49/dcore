#include <dcore/Core/DynamicVector.hpp>
#include <dcore/Core/Memory.hpp>
#include <dcore/Core/Allocators/Common.hpp>

namespace dstd
{
    void DynamicVector::RawAdd(const void *value)
    {
        Data_ = CommonAllocator::ReAllocN((Byte*)Data_, (Size_ + 1) * ItemSize_);
        CopyBuffer(ItemSize_, (Byte*)(Data_) + Size_ * ItemSize_, (Byte*)value);
        Size_ += 1;
    }

    void *DynamicVector::RawGet(USize idx) const
    {
        return (Byte*)Data_ + idx * ItemSize_;
    }
}
