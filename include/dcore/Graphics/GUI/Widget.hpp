#pragma once
#include <dcore/Graphics/GUI/GuiGraphics.hpp>
#include <dcore/Graphics/GUI/GraphicsData.hpp>
#include <dcore/Event/EventManager.hpp>
#include <dcore/Graphics/GUI/Event.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Uni.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace dcore::graphics::gui
{
	/**
	 * A widget is a separate component in the user interface (not to be confused with entity
	 * components.)
	 * */
	class Widget
	{
	public:
		Widget();
		virtual ~Widget();

		const glm::vec2 &GetPosition() const;
		void SetPosition(const glm::vec2 &newPosition);

		float GetRotation() const;
		void SetRotation(float newRotation);

		const glm::vec2 &GetSize() const;
		void SetSize(const glm::vec2 &newSize);

		/** Returns the quad that is used to render the widget. */
		const Quad &GetQuad() const;

		template<typename T>
		T *AllocChild();

		template<typename T, typename... Args>
		T *CreateChild(Args &&...args);

		bool IsFocused() const;
		void Focus();

	protected:
		/** Called when the widget is initialized (only called by DoInitialize_)*/
		virtual void Initialize() = 0;

		/** Called when the widget is deinitialized (only called by DoDeInitialize_)*/
		virtual void DeInitialize() = 0;

		/** Called when the widget is rendered (only called by DoRender_)*/
		virtual void Render(GuiGraphics *g) = 0;

		/** Called when the widget has received an event (only called by DoHandleEvent_)*/
		virtual void HandleEvent(event::Event *e) = 0;

		/** Non-const version of GetQuad() */
		Quad &GetQuad();

	private:
		friend class GuiManager;

		/** Finds a widget that the user has clicked on. */
		Widget *GetFocusWithin_(FocusEvent *e);

		/** Sets the defaults for all of the fields. */
		void SetDefaults_();

		/** Called by the GuiManager when the widget is initialized.
		 * Handles children logic and calls Initialize(). */
		void DoInitialize_();

		/** Called by the GuiManager when the widget is deinitialized.
		 * Handles children logic and calls DeInitialize() */
		void DoDeInitialize_();

		/** Called by the GuiManager when the widget is rendered.
		 * Handles children logic and calls Render() */
		void DoRender_(GuiGraphics *g);

		/** Called by the GuiManager when the widget has received an event.
		 *  Handles children logic and calls HandleEvent() */
		void DoHandleEvent_(event::Event *e);

		bool IsFocused_;
		Quad Quad_;
		DCORE_REF Widget *Parent_;
		std::vector<DCORE_OWN Widget *> Children_;
	};

	template<typename T>
	T *Widget::AllocChild()
	{
		auto placement = reinterpret_cast<T *>(new byte[sizeof(T)]);
		Children_.push_back(placement);
		return placement;
	}

	template<typename T, typename... Args>
	T *Widget::CreateChild(Args &&...args)
	{
		Children_.push_back(new T(std::forward<Args>(args)...));
		return dynamic_cast<T *>(Children_.back());
	}
} // namespace dcore::graphics::gui
