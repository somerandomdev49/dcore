#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <picojson.hpp>

namespace dcore::data
{
    // union JsonValue;
    // typedef std::unordered_map<std::string, JsonValue> JsonObject;
    // typedef std::vector<JsonValue> JsonArray;
    // typedef std::string JsonString;
    // typedef double JsonNumber;
    // union JsonValue { JsonNumber number; JsonString string; JsonArray array; JsonObject object; };

    
    /** Class for outpting serialized json data. */
    class FileOutput
    {
    public:
        FileOutput(const picojson::value &root)

        const std::string &GetPath() const;
    private:
        std::string Path_;
        picojson::value Root_;
    };
}
