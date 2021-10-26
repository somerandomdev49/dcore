#pragma once
#include <glm/glm.hpp>
#include <dcore/Event/InputManager.hpp>

namespace dcore::platform
{
	/** Virtual class representing a window. Multiple implementations can exist. */
	class Frame
	{
	public:
		virtual ~Frame();
		virtual void Initialize(const glm::ivec2 &size) = 0;
		virtual bool ShouldEnd()                        = 0;
		virtual void OnBeginFrame()                     = 0;
		virtual void OnEndFrame()                       = 0;
		virtual void DeInitialize()                     = 0;
		virtual float GetCurrentTime()                  = 0;

		virtual bool CheckKeyPressed(event::KeyCode key) = 0;
		virtual bool CheckMouseButtonPressed(int button) = 0;

		const glm::ivec2 &GetSize() const;
		void SetSize(const glm::ivec2 &newSize);

	protected:
		glm::ivec2 Size_;
	};
} // namespace dcore::platform
