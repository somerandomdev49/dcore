#pragma once
#include <typeinfo>

// TODO: Doesn't work inside of nested namespaces for some reason, try to fix!

namespace dcore
{
	// Generic implementation, may return bad results.
	template<typename T>
	const char *TypeName()
	{
		return typeid(T).name();
	}
} // namespace dcore

#define DCORE_REFL_TYPE(T)              \
	template<>                          \
	const char * ::dcore::TypeName<T>() \
	{                                   \
		return #T;                      \
	}
