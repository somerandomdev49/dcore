#pragma once
#include <glm/glm.hpp>

namespace dcore::platform
{
	/** Virtual class representing a window. Multiple implementations can exist. */
	class Frame
	{
	public:
		virtual ~Frame();
		virtual void Initialize(const glm::ivec2 &size);
		virtual bool ShouldEnd();
		virtual void OnBeginFrame();
		virtual void OnEndFrame();
		virtual void DeInitialize();

		virtual bool CheckKeyPressed(int key); 
		virtual bool CheckMouseButtonPressed(int button);

		const glm::ivec2 &GetSize() const;
		void SetSize(const glm::ivec2 &newSize);
		
	protected:
		glm::ivec2 Size_;
	};
}
