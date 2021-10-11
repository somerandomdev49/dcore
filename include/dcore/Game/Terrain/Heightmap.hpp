#pragma once
#include <glm/glm.hpp>
#include <dcore/Resource/ResourceManager.hpp>

namespace dg::terrain
{
    class Heightmap
    {
        static void Register();
    private:
        void Load(const std::string &id, const std::string &location, dcore::resource::ResourceManager *res);

        glm::vec2 Size_;
        float *Data_;
    };

    class HeightmapRegion
    {
        const glm::vec2 &GetMin() const;
        const glm::vec2 &GetMax() const;
        Heightmap *GetSource();

        float At(const glm::vec2 &pos) const;
    private:
        glm::vec2 Min_, Max_;
        dcore::resource::Resource<Heightmap> Source_;
    };
}
