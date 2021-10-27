#include <dcore/Graphics/GUI/Widget.hpp>

using namespace dcore::graphics::gui;

void Widget::SetDefaults_()
{
	IsFocused_     = false;
	Parent_        = nullptr;
	Quad_.Color    = glm::vec4(1.0f);
	Quad_.Texture  = nullptr;
	Quad_.Position = glm::vec2(0.0f, 0.0f);
	Quad_.Scale    = glm::vec2(1.0f, 1.0f);
	Quad_.Rotation = 0.0f;
}

void Widget::DoInitialize_()
{
	SetDefaults_();
	Initialize(); // Call the user method.
	for(const auto &child : Children_) child->DoInitialize_();
}

void Widget::DoDeInitialize_()
{
	for(const auto &child : Children_) child->DoDeInitialize_();
	DeInitialize(); // Call the user method.
	SetDefaults_();
}

void Widget::DoHandleEvent_(event::Event *e)
{
	if(e->Type == gui::EventTypeFocus)
	{
		auto focused = GetFocusWithin_(reinterpret_cast<FocusEvent*>(e));
		if(focused == this) IsFocused_ = true;
		// TODO: separate method for focus event, so we can call it directly
		// and spare the event type check.
		else focused->DoHandleEvent_(e); 
	}
	else
	{
		// Call the user method.
		HandleEvent(e);
	}
}

void Widget::DoRender_(GuiGraphics *g)
{
	// nothing here fow now. (for now?)
	Render(g); // Call the user method.
}
