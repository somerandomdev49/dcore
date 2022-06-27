#pragma once
#include <dcore/Core/Type.hpp>

namespace dstd
{
	template<typename T>
	struct Span
	{
		T *Data;
		dstd::USize Count;

		// TODO: Iterators
	};
} // namespace dstd
