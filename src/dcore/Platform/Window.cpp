#include <dcore/Platform/Window.hpp>

using namespace dcore::platform;

Frame::~Frame() {}

const glm::ivec2 &Frame::GetSize() const { return Size_; }
void              Frame::SetSize(const glm::ivec2 &v) { Size_ = v; }
