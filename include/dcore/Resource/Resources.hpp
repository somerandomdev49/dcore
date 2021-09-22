#pragma once
#include <string>
#include <string_view>

namespace dcore::resource
{
    /** Parent class for Resource users/providers */
    class Resources
    {
    public:
        /** Returns the root path */
        const std::string &GetRootPath() const;
    protected:
        Resources(const std::string_view &root);

        /** Sets the root path. */
        void SetRootPath(const std::string_view &newRoot);

        /** Returns the full path of a resource */
        std::string FullPath(const std::string_view &relative);

    private:
        std::string RootPath_;
    };
}
