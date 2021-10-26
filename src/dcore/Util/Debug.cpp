#include <dcore/Util/Debug.hpp>
#include <cxxabi.h>

std::string dcore::util::Debug::Demangle(const std::string &name)
{
	char *realname;
	int status;
	realname = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
	if(status == 0)
	{
		std::string r(realname);
		free(realname);
		return r;
	}
	return "<error>";
}
