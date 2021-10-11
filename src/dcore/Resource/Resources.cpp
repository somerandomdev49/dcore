#include <dcore/Resource/Resources.hpp>
using namespace dcore::resource;

const std::string &Resources::GetRootPath() const
{
	return RootPath_;
}

void Resources::SetRootPath(const std::string &newRoot)
{
	RootPath_ = newRoot;
}

Resources::Resources(const std::string &root) : RootPath_(root)
{
}

std::string Resources::FullPath(const std::string &location)
{
	return RootPath_ + '/' + location;
}
