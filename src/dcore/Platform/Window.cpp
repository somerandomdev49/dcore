#include <dcore/Platform/Window.hpp>

namespace dcore::platform
{
	Frame::~Frame() = default;

	const glm::ivec2 &Frame::GetSize() const { return Size_; }
	void Frame::SetSize(const glm::ivec2 &newSize) { Size_ = newSize; }

	void Frame::RegisterResourceManager(resource::ResourceLoader *rl)
	{
		rl->RegisterResourceType<RCursor>("Cursor");
		resource::ResourceManager::Instance()->RegisterConstructor<RCursor>(&RCursor_Constructor);
		resource::ResourceManager::Instance()->RegisterDeConstructor<RCursor>(&RCursor_DeConstructor);
	}
} // namespace dcore::platform
