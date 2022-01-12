#pragma once
#include <dcore/Graphics/GUI/Widget.hpp>
#include <dcore/Graphics/GUI/Font.hpp>

namespace dcore::graphics::gui::common
{
	/**
	 * @brief Fullscreen vertical scrolling text
	 */
	class ScrollingText : public Widget
	{
	public:
		ScrollingText(const std::vector<std::string> &lines, const glm::vec3 &color);
		ScrollingText(const std::vector<std::string> &lines,
		              const dcore::resource::Resource<dcore::graphics::RTexture> &backgroundTexture,
		              const glm::vec3 &tint = glm::vec3(1));

		virtual void Initialize() override;
		virtual void DeInitialize() override;
		virtual void Render(GuiGraphics *g) override;
		virtual void HandleEvent(event::Event *e) override;

	private:
		Font *Font_; // TODO: Have a default font somewhere.

		bool HasBackground_;
		dcore::resource::Resource<dcore::graphics::RTexture> Background_;
		glm::vec3 BackgroundTint_;

		std::vector<std::string> Lines_;
		float CurrentOffset_;

		void Construct_();
	};
} // namespace dcore::graphics::gui::common
