#pragma once
#include <dcore/Core/Type.hpp>

namespace dcore::net
{
	template<typename T>
	struct Vector2 { T X, Y; };

	template<typename T>
	struct Vector3 { T X, Y, Z; };
	
	template<typename T>
	struct Vector4 { T X, Y, Z, W; };

	using Vector2f   = Vector2<float>;
	using Vector3f   = Vector3<float>;
	using Vector4f   = Vector4<float>;
	using Vector2d   = Vector2<double>;
	using Vector3d   = Vector3<double>;
	using Vector4d   = Vector4<double>;
	using Vector2u32 = Vector2<dstd::UInt32>;
	using Vector3u32 = Vector3<dstd::UInt32>;
	using Vector4u32 = Vector4<dstd::UInt32>;
	using Vector2u16 = Vector2<dstd::UInt16>;
	using Vector3u16 = Vector3<dstd::UInt16>;
	using Vector4u16 = Vector4<dstd::UInt16>;
	using Vector2u8  = Vector2<dstd::UInt8>;
	using Vector3u8  = Vector3<dstd::UInt8>;
	using Vector4u8  = Vector4<dstd::UInt8>;
}
