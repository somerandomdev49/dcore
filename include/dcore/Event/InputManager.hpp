#pragma once
#include <dcore/Launch.hpp>
#include <glm/glm.hpp>

namespace dcore::platform { class Context; }

// TODO: Add actions
namespace dcore::event
{
	/** Key code (from GLFW) */
	enum KeyCode
	{
		K_Unknown      = -1,
		K_Space        = 32,
		K_Apostrophe   = 39, /* ' */
		K_Comma        = 44, /* , */
		K_Minus        = 45, /* - */
		K_Period       = 46, /* . */
		K_Slash        = 47, /* / */
		K_0            = 48,
		K_1            = 49,
		K_2            = 50,
		K_3            = 51,
		K_4            = 52,
		K_5            = 53,
		K_6            = 54,
		K_7            = 55,
		K_8            = 56,
		K_9            = 57,
		K_Semicolon    = 59, /* ; */
		K_Equal        = 61, /* = */
		K_A            = 65,
		K_B            = 66,
		K_C            = 67,
		K_D            = 68,
		K_E            = 69,
		K_F            = 70,
		K_G            = 71,
		K_H            = 72,
		K_I            = 73,
		K_J            = 74,
		K_K            = 75,
		K_L            = 76,
		K_M            = 77,
		K_N            = 78,
		K_O            = 79,
		K_P            = 80,
		K_Q            = 81,
		K_R            = 82,
		K_S            = 83,
		K_T            = 84,
		K_U            = 85,
		K_V            = 86,
		K_W            = 87,
		K_X            = 88,
		K_Y            = 89,
		K_Z            = 90,
		K_LeftBracket  = 91, /* [ */
		K_Backslash    = 92, /* \ */
		K_RightBracket = 93, /* ] */
		K_GraveAccent  = 96, /* ` */
		K_Escape       = 256,
		K_Enter        = 257,
		K_Tab          = 258,
		K_Backspace    = 259,
		K_Insert       = 260,
		K_Delete       = 261,
		K_Right        = 262,
		K_Left         = 263,
		K_Down         = 264,
		K_Up           = 265,
		K_PageUp       = 266,
		K_PageDown     = 267,
		K_Home         = 268,
		K_End          = 269,
		K_CapsLock     = 280,
		K_ScrollLock   = 281,
		K_NumLock      = 282,
		K_PrintScreen  = 283,
		K_Pause        = 284,
		K_F1           = 290,
		K_F2           = 291,
		K_F3           = 292,
		K_F4           = 293,
		K_F5           = 294,
		K_F6           = 295,
		K_F7           = 296,
		K_F8           = 297,
		K_F9           = 298,
		K_F10          = 299,
		K_F11          = 300,
		K_F12          = 301,
		K_F13          = 302,
		K_F14          = 303,
		K_F15          = 304,
		K_F16          = 305,
		K_F17          = 306,
		K_F18          = 307,
		K_F19          = 308,
		K_F20          = 309,
		K_F21          = 310,
		K_F22          = 311,
		K_F23          = 312,
		K_F24          = 313,
		K_F25          = 314,
		K_Kp0          = 320,
		K_Kp1          = 321,
		K_Kp2          = 322,
		K_Kp3          = 323,
		K_Kp4          = 324,
		K_Kp5          = 325,
		K_Kp6          = 326,
		K_Kp7          = 327,
		K_Kp8          = 328,
		K_Kp9          = 329,
		K_KpDecimal    = 330,
		K_KpDivide     = 331,
		K_KpMultiply   = 332,
		K_KpSubtract   = 333,
		K_KpAdd        = 334,
		K_KpEnter      = 335,
		K_KpEqual      = 336,
		K_LeftShift    = 340,
		K_LeftControl  = 341,
		K_LeftAlt      = 342,
		K_LeftSuper    = 343,
		K_RightShift   = 344,
		K_RightControl = 345,
		K_RightAlt     = 346,
		K_RightSuper   = 347,
		K_Menu         = 348,
	};


	class InputManager
	{
	public:
		void Initialize();
		void DeInitialize();
		bool IsKeyPressed(event::KeyCode key);
		bool IsMousePressed(int button);
		float GetScrollX() const { return Scroll_.x; }
		float GetScrollY() const { return Scroll_.y; }

		void SetScroll(glm::vec2 newScroll) { Scroll_ = newScroll; }
		const glm::vec2 &GetScroll() const { return Scroll_; }

		void SetMousePosition(glm::vec2 newPosition) { MousePosition_ = newPosition; }
		const glm::vec2 &GetMousePosition() const { return MousePosition_; }

		static InputManager *Instance();

	private:
		friend class platform::Context;
		friend class launch::Launch;
		static void SetInstance(InputManager *newInstance);
		glm::vec2 MousePosition_;
		glm::vec2 Scroll_;
	};
} // namespace dcore::event
