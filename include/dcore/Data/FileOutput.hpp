#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <picojson.hpp>
#include <dcore/Resource/Resources.hpp>

namespace dcore::data
{
    /** Class responsible from writing in the actual format. */
    class OutputAdapter
    {
    public:
        virtual ~OutputAdapter();
        virtual void Write(const std::string &where, const picojson::value &value) = 0;
    };
    
    /** Class for outpting serialized json data. */
    class FileOutput : public resource::Resources
    {
    public:
        FileOutput(const std::string &root, const std::string &initialPath);

        /** Sets the value of the file. */
        void Set(const picojson::value &value);

        /** Writes with the adapter. */
        void Write();

        OutputAdapter *GetAdapter() const;
        void SetAdapter(OutputAdapter *newAdapter);

        void SetPath(const std::string &newPath);
        const std::string &GetPath() const;
    private:
        std::string Path_;
        picojson::value RootValue_;
        OutputAdapter *Adapter_;
    };
}
