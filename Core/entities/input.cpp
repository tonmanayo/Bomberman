#include <window.hpp>

namespace Zion
{
	bool    Input::_keys[MAX_KEYS];
	bool    Input::_mouse[MAX_BUTTONS];
	double  Input::_mouseX = 0.0f;
	double  Input::_mouseY = 0.0f;
	bool    Input::isPoll = false;

	bool (*Input::mouseCallback2)(int, int, int) = nullptr;
	bool (*Input::keyCallback2)(int, int, int, int) = nullptr;
	bool (*Input::cursorPositionCallback2)(int, int) = nullptr;

	Input::Input()
	{
		for (bool& key : _keys)
			key = false;
		for (bool& mouse : _mouse)
			mouse = false;
	}

	Input::Input(const Input &rhs) { *this = rhs; }
	Input& Input::operator=(const Input &rhs) { return *this; }

	void Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		_keys[key] = action != GLFW_RELEASE;
		if (keyCallback2 != nullptr)
			keyCallback2(key, scancode, action, mods);
		isPoll = true;
		std::cout << "key call back" << std::endl;
	}

	void Input::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
	{
		_mouse[button] = action != GLFW_RELEASE;
		if (mouseCallback2 != nullptr)
			mouseCallback2(button, action, mods);
		isPoll = true;
	}

	void Input::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
	{
		_mouseX = xpos;
		_mouseY = ypos;
		if (cursorPositionCallback2 != nullptr)
			cursorPositionCallback2(xpos, ypos);
		isPoll = true;
	}

	bool Input::getKeyStatus(int key) const
	{
		if (key >= MAX_KEYS)
			return false;
		return _keys[key];
	}

	bool Input::getMouseButtonStatus(int button) const
	{
		if (button >= MAX_BUTTONS)
			return false;
		return _mouse[button];
	}

	double Input::getMousePosX() const { return _mouseX; }
	double Input::getMousePosY() const { return _mouseY; }
}