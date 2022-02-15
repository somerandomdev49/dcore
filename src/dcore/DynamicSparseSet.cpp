#include <dcore/Core/DynamicSparseDataSet.hpp>

namespace dstd
{
	void *DynamicSparseSet::RawGet(dstd::USize index) { return Packed_.RawGet(Sparse_[index]); }

	const void *DynamicSparseSet::RawGet(dstd::USize index) const { return Packed_.RawGet(Sparse_.at(index)); }
} // namespace dstd
