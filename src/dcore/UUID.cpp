#include <dcore/Core/UUID.hpp>
#include <cstdio>

// TODO: Convert to actual util function
// Taken from linux/lib/hexdump.c (https://code.woboq.org/linux/linux/lib/hexdump.c.html#hex_to_bin)
static int HexToBin(char ch)
{
	if((ch >= '0') && (ch <= '9')) return ch - '0';

	ch = std::tolower(ch);
	if((ch >= 'a') && (ch <= 'f')) return ch - 'a' + 10;

	return -1;
}

// Taken (and modified) from linux/lib/uuid.c (https://code.woboq.org/linux/linux/lib/uuid.c.html#__uuid_parse)
static void ParseUUID(const std::string &uuid, dstd::UUID &u)
{
	static const dstd::Byte si[16] = {0, 2, 4, 6, 9, 11, 14, 16, 19, 21, 24, 26, 28, 30, 32, 34};

	for(dstd::UInt8 i = 0; i < 16; i++)
	{
		int hi     = HexToBin(uuid[si[i] + 0]);
		int lo     = HexToBin(uuid[si[i] + 1]);
		u.Bytes[i] = (hi << 4) | lo;
	}
}

namespace dstd
{
	void UUID::Parse(UUID &to, const std::string &source) { ParseUUID(source, to); }

	std::string UUID::ToString() const
	{
		// TODO: Implement dstd::UUID::ToString()
		return "<TODO: Implement UUID::ToString()>";
	}
} // namespace dstd
