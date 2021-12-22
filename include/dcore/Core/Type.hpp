#pragma once

namespace dstd
{
#ifdef DCORE_STD_64BIT
	using Int8   = signed char;
	using UInt8  = unsigned char;
	using Int16  = signed short;
	using UIn16  = unsigned short;
	using Int32  = signed int;
	using UInt32 = unsigned int;
	using Int64  = signed long;
	using UInt64 = unsigned long;
	using USize  = UInt64;
#else
#error DragonCore only supports 64 bit platorms (define DCORE_STD_64BIT)
#endif
} // namespace dstd
