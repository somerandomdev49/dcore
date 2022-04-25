#include <dcore/Core/DynamicSparseDataSet.hpp>
#include <dcore/Core/Log.hpp>

namespace dstd
{
	void *DynamicSparseSet::RawGet(USize index)
	{
		return (dstd::Byte*)Packed_.RawGet(Sparse_[index]) + sizeof(Element);
	}

	const void *DynamicSparseSet::RawGet(USize index) const { return Packed_.RawGet(Sparse_.at(index)); }

	bool DynamicSparseSet::Contains(USize index) const
	{
		auto found = Sparse_.find(index);
		return found != Sparse_.end() && found->second < Packed_.GetSize();
	}

	void *DynamicSparseSet::RawSet(USize index, const void *value)
	{
		// LOG_F(INFO, "DynamicSparseSet::RawSet(index: %lu, value: ...)", index);

		// Set the index in the sparse set
		Sparse_[index] = Packed_.GetSize();

		// Allocate space for a new item
		auto *item = Packed_.AllocAdd<Byte>();

		// Copy the element header (index into the sparse set)
		CopyBuffer(sizeof(Element), item, (const Byte *)&index);

		// Copy actual value into the dynamic vector.
		USize valueSize = Packed_.GetItemSize() - sizeof(Element);
		CopyBuffer(valueSize, item + sizeof(Element), (const Byte *)&value);

		return item + sizeof(Element);
	}

} // namespace dstd
