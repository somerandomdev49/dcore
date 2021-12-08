#pragma once
#include <type_traits>
#include <utility>

namespace dcore::util
{
#define DCORE_HAS_MEMBER(NAME)                                                                                \
	template<typename Class, typename Enabled = void>                                                         \
	struct has_##NAME##_s                                                                                     \
	{                                                                                                         \
		static constexpr bool value = false;                                                                  \
	};                                                                                                        \
	template<typename Class>                                                                                  \
	struct has_##NAME##_s<Class, std::enable_if_t<std::is_member_function_pointer_v<decltype(&Class::NAME)>>> \
	{                                                                                                         \
		static constexpr bool value = std::is_member_function_pointer_v<decltype(&Class::NAME)>;              \
	};                                                                                                        \
	template<typename Class>                                                                                  \
	constexpr bool has_##NAME()                                                                               \
	{                                                                                                         \
		return has_##NAME##_s<Class>::value;                                                                  \
	};
} // namespace dcore::util
