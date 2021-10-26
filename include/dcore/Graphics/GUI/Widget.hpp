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
	 * A widget is a separate component in the user interface (not to be confused with entity components.)
	 * */
	class Widget
	{
	public:
		// Note: to fully (de)initialize a widget, it's constructor/destructor must be called.

		Widget();
		virtual ~Widget();

		virtual void Initialize()           = 0;
		virtual void DeInitialize()         = 0;
		virtual void Render(GuiGraphics *g) = 0;
		virtual void Event(event::Event *e) = 0;

		const glm::vec2 &GetPosition() const;
		void SetPosition(const glm::vec2 &newPosition);

		float GetRotation() const;
		void SetRotation(float newRotation);

		const glm::vec2 &GetSize() const;
		void SetSize(const glm::vec2 &newSize);

		template<typename T>
		T *AllocChild();

		template<typename T, typename... Args>
		T *CreateChild(Args &&...args);

		bool IsFocused() const;
		void Focus();

	private:
		bool IsFocused_;
		Quad Quad_;
		std::vector<RTexture *> Texture_;
		DCORE_REF Widget *Parent_;
		std::vector<DCORE_OWN Widget *> Children_;
	};

	template<typename T>
	T *Widget::AllocChild()
	{
		auto placement = new byte[sizeof(T)];
		Children_.push_back(placement);
		return placement;
	}

	template<typename T, typename... Args>
	T *Widget::CreateChild(Args &&...args)
	{
		Children_.push_back(new T(std::forward<Args>(args)...));
		return reinterpret_cast<T *>(Children_.back());
	}
} // namespace dcore::graphics::gui
