#pragma once
#include <dcore/Event/InputManager.hpp>
#include <dcore/Event/EventManager.hpp>
#include <glm/glm.hpp>

namespace dcore::graphics::gui
{
	enum GuiEventTypes
	{
		EventTypeFocus     = 200 + 4,
		EventTypeTextInput = 200 + 5
	};

	struct FocusEvent : public event::Event
	{
		glm::vec2 MousePosition;
		FocusEvent(const glm::vec2 &mousePosition)
		    : event::Event(EventTypeFocus), MousePosition(mousePosition)
		{
		}
	};

	struct TextInputEvent : public event::Event
	{
		int Value;
		TextInputEvent(int value) : event::Event(EventTypeTextInput), Value(value) {}
	};
} // namespace dcore::graphics::gui
