#pragma once
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Renderer/Impl/OpenGL/Vao.hpp>
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <dcore/Renderer/Config.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace dcore::graphics
{
	class Renderer;

	namespace impl::opengl
	{
		struct SkeletalMesh
		{
			Vao Vao_;
		};
	}; // namespace impl::opengl

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
		impl::DCORE_GRAPHICS_IMPL::SkeletalMesh Data_;
	};
} // namespace dcore::graphics
