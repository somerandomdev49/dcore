#pragma once
#include <dcore/Core/Type.hpp>
#include <dcore/Core/Span.hpp>
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace dcore::physics
{
	namespace rp3d = reactphysics3d;

	/**
	 * @brief Root physics class (there is a global instance). Contains the @ref reactphysics3d::PhysicsCommon.
	 */
	class Physics
	{
	public:
		static Physics *Instance();
		rp3d::PhysicsCommon &GetCommon() { return PhysicsCommon_; }
		const rp3d::PhysicsCommon &GetCommon() const { return PhysicsCommon_; }

	private:
		rp3d::PhysicsCommon PhysicsCommon_;
	};

	namespace detail
	{
		inline glm::vec3 Convert(rp3d::Vector3 v) { return {v.x, v.y, v.z}; }
		inline glm::quat Convert(rp3d::Quaternion v) { return {v.w, v.x, v.y, v.z}; }
		inline rp3d::Vector3 Convert(glm::vec3 v) { return {v.x, v.y, v.z}; }
		inline rp3d::Quaternion Convert(glm::quat v) { return {v.x, v.y, v.z, v.w}; }
	} // namespace detail
} // namespace dcore::physics
