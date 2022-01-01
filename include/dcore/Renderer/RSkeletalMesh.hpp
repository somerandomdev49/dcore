#pragma once
#include <dcore/Renderer/Config.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace dcore::graphics
{
	class RJoint
	{
	private:
		int Index;
		glm::mat4 Matrix;
		std::vector<int> ChildrenIndices_;
		// RJoint *Parent;
	};

	class RSkeletalMesh
	{
		std::vector<RJoint> Joints_;
		int JointCount_;

		friend class Renderer;
		friend class RenderResourceManager;
		void *Data_;
	};
} // namespace dcore::graphics
