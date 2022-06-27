#pragma once
#include <glm/glm.hpp>
#include <dcore/Event/InputManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <string>

namespace dcore::platform
{
	/** Cursor image resource */
	struct RCursor
	{
		void *cursor;
		void *image;
	};

	// TODO: Change to a static implementation!

	/** Abstract class representing a window. Multiple implementations can exist. */
	class Frame
	{
	public:
		enum CursorState
		{
			CursorState_Normal,
			CursorState_Down,
			CursorState_Grab,
			CursorState_Point,
			CursorState_Max_
		};

		virtual ~Frame();
		virtual void Initialize(const glm::ivec2 &size) = 0;
		virtual void OnBegin()                          = 0;
		virtual bool ShouldEnd()                        = 0;
		virtual void OnBeginFrame()                     = 0;
		virtual void OnEndFrame()                       = 0;
		virtual void DeInitialize()                     = 0;
		virtual float GetCurrentTime()                  = 0;

		virtual bool CheckKeyPressed(event::KeyCode key) = 0;
		virtual bool CheckMouseButtonPressed(int button) = 0;
		virtual void SetCursorState(CursorState state)   = 0;

		const glm::ivec2 &GetSize() const { return Size_; }

		void SetCursor(CursorState state, void *cursor) { CursorMap_[state] = cursor; }

		virtual glm::ivec2 GetFramebufferSize();
		virtual void SetSize(glm::ivec2 newSize);

		static void RegisterResourceManager(resource::ResourceLoader *rl);

	protected:
		glm::ivec2 Size_;
		void *CursorMap_[CursorState_Max_];

	private:
		static void RCursor_Constructor(const std::string &path, void *placement);
		static void RCursor_DeConstructor(void *placement);
	};
} // namespace dcore::platform
