#pragma once
#include <dcore/Event/InputManager.hpp>
#include <unordered_map>

// TODO? Fix statically setting enums!

namespace dcore::event
{
	enum CommonEventTypes
	{
		EventTypeClick       = 100 + 0,
		EventTypeDoubleClick = 100 + 1,
		EventTypeKeyDown     = 100 + 2,
		EventTypeKeyUp       = 100 + 3
	};

	struct Event
	{
		int Type;
	};

	struct ClickEvent : public Event
	{
		ClickEvent(int button) : button(button) {}
		int button;
	};

	struct DoubleClickEvent : public Event
	{
		DoubleClickEvent(int button) : button(button) {}
		int button;
	};

	struct KeyDownEvent : public Event
	{
		KeyDownEvent(event::KeyCode key) : key(key) {}
		event::KeyCode key;
	};

	struct KeyUpEvent : public Event
	{
		KeyUpEvent(event::KeyCode key) : key(key) {}
		event::KeyCode key;
	};

	class EventManager
	{
	public:
		using EventHandler = bool (*)(void *sender, Event *event);

		void AddEventHandler(int type, EventHandler handler);

	private:
	};
} // namespace dcore::event
