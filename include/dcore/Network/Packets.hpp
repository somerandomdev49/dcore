#pragma once
#include <dcore/Core/Type.hpp>
#include <dcore/Network/Types.hpp>

#include <initializer_list>

namespace dcore::net
{
	namespace PacketType
	{
		/** Outgoing packets from client. */
		enum Client : dstd::Byte
		{
			JoinReq  = 0x00, // Requests server for client to join
			LeaveReq = 0x01, // Requests server for client to leave
			Left     = 0x02, // Specifies that client left (e.g. closed game window)
			Moved    = 0x03, // Specifies that the client moved
		};
		
		/** Outgoing packets from server. */
		enum Server : dstd::Byte
		{
			None     = (dstd::Byte)-1, // Not an actual packet, helper
			OK       = 0x00, // Server allows an action to happen (or aknowledges)
			Reject   = 0x01, // Server doesnt allow an action to happen
			JoinRes  = 0x02, // Response to JoinReq
			LeaveRes = 0x03, // Response to LeaveReq
			MoveRej  = 0x04, // Rejection of Moved
		};
	}

	//             client? -> possible response packets
	//             server? -> packets this can respond to
	//       type of packet   `------------.vvvvvv-------`
	template<dstd::UInt8 PT, dstd::UInt8 ...ResPTs>
	struct Packet
	{
		static constexpr dstd::UInt8 PacketType = PT;
		static constexpr std::initializer_list<dstd::Byte> ResPackets = { ResPTs... };
		dstd::UInt8 Type = PT;
	};

	struct PacketJoinReq : Packet<PacketType::JoinReq, PacketType::JoinRes>
	{
		dstd::UInt8 Character;
	};

	struct PacketLeaveReq : Packet<PacketType::LeaveReq, PacketType::LeaveRes>
	{

	};

	struct PacketLeft : Packet<PacketType::Left, PacketType::None>
	{
		enum Reason : dstd::UInt8
		{
			CloseWindow,
			ConnectionLost,
		} Reason;
	};

	struct PacketMoved : Packet<PacketType::Moved, PacketType::OK, PacketType::MoveRej>
	{
		Vector2f NewPosition;
	};
}
