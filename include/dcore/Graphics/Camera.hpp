#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace dcore::graphics
{
	class Camera
	{
	public:
		/**
		 * @param fov Vertical field of view of the camera
		 * @param aspect Aspect ratio of the view
		 * @param near Near depth
		 * @param far Far depth
		 */
		Camera(float fov = 70.f, float aspect = 4.f / 3.f, float near = 0.1f, float far = 200.f);

		const glm::mat4 &GetViewMatrix();
		const glm::mat4 &GetProjMatrix();

		float GetFov() const;
		float GetAspectRatio() const;
		float GetNearZ() const;
		float GetFarZ() const;

		void SetFov(float newFov);
		void SetAspectRatio(float newAspectRatio);
		void SetNearZ(float newNearZ);
		void SetFarZ(float newFarZ);

		const glm::vec3 &GetPosition() const;
		void SetPosition(const glm::vec3 &newPosition);

		const glm::quat &GetRotation() const;
		void SetRotation(const glm::quat &newRotation);

		void RecalcViewMatrix();
		void RecalcProjMatrix();

	private:
		glm::vec3 Position_;
		glm::quat Rotation_;
		glm::mat4 ViewMatrix_, ProjMatrix_;
		float Fov_, Aspect_, NearZ_, FarZ_;
		bool DirtyView_, DirtyProj_;
	};
} // namespace dcore::graphics
