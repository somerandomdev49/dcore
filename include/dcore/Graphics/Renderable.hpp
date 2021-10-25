#pragma once
#include <glm/glm.hpp>

namespace dcore::graphics
{
	class Renderable
	{
	public:
		const glm::mat4 &GetTransform() const;
		void             SetTransform(const glm::mat4 &m);

	private:
		glm::mat4 Transform_;
	};
} // namespace dcore::graphics
