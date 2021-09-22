#include <dcore/Resource/Resources.hpp>
using namespace dcore::resource;

const std::string &Resources::GetRootPath() const
{ return RootPath_; }

void Resources::SetRootPath(const std::string_view &newRoot)
{ RootPath_ = newRoot; }


Resources::Resources(const std::string_view &root)
    : RootPath_(root) {}

std::string Resources::FullPath(const std::string_view &location)
{
    return (RootPath_ + '/') + std::string(location);
}
