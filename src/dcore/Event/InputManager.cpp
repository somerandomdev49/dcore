#include <dcore/Event/InputManager.hpp>
#include <dcore/Platform/Platform.hpp>

using namespace dcore::event;

void InputManager::Initialize() {
}
bool InputManager::IsKeyPressed(event::KeyCode key) {
	return platform::Context::Instance()->IsKeyPressed(key);
}
bool InputManager::IsMousePressed(int button) {
	return platform::Context::Instance()->IsMousePressed(button);
}
void InputManager::DeInitialize() {
}

static InputManager *inpMngr;
InputManager *InputManager::Instance() {
	if(inpMngr == nullptr) inpMngr = new InputManager();
	return inpMngr;
}

void InputManager::SetInstance(InputManager *newInstance) {
	inpMngr = newInstance;
}
