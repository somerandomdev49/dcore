#pragma once
#include <dcore/Core/Type.hpp>

#include <string>

namespace dstd
{
	// Represents a standard UUID (16 bytes)
	struct UUID
	{
		Byte Bytes[16];

		static void Parse(UUID &to, const std::string &source);
		std::string ToString() const;
	};
} // namespace dstd
