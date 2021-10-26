#pragma once
#include <dcore/Event/InputManager.hpp>
#include <dcore/Event/EventManager.hpp>

namespace dcore::graphics::gui
{
	struct FocusEvent : public event::Event
	{
		bool IsFocused;
		FocusEvent(bool isFocused) : event::Event(), IsFocused(isFocused) {}
	};

	struct TextInputEvent : public event::Event
	{
		int Value;
		TextInputEvent(int value) : Value(value) {}
	};
} // namespace dcore::graphics::gui
