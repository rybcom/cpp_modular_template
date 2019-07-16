#include "Keyboard.h"
#include "KeyState.h"
#include <vector>

namespace user_input
{
	namespace
	{

		using KeyStateList = std::vector<KeyState>;

		KeyStateList _keyStateList;

		std::vector<std::function<void(Key)>> _keyPressedCallbacks;
		std::vector<std::function<void(Key)>> _keyUpCallbacks;
		std::vector<std::function<void(Key)>> _keyDownCallbacks;

		Key const KeyList[] =
		{
			Key::Escape,
			Key::Left, Key::Right, Key::Up, Key::Down,
			Key::D0, Key::D1, Key::D2, Key::D3, Key::D4, Key::D5, Key::D6, Key::D7, Key::D8, Key::D9,
			Key::A, Key::B, Key::C, Key::D, Key::E, Key::F, Key::G, Key::H, Key::I, Key::J, Key::K, Key::L,
			Key::M, Key::N, Key::O, Key::P, Key::Q, Key::R, Key::S, Key::T, Key::U, Key::V, Key::W, Key::X, Key::Y, Key::Z,
			Key::Delete,
			Key::LeftCtrl,Key::LeftAlt,
			Key::NumPad0, Key::NumPad1, Key::NumPad2, Key::NumPad3, Key::NumPad4, Key::NumPad5,
			Key::NumPad6, Key::NumPad7, Key::NumPad8, Key::NumPad9, Key::Add,Key::Subtract

		};

		void initKeyStates()
		{
			for (Key key : KeyList)
			{
				_keyStateList.emplace_back(key);
			}
		}

		struct KeyboardInitializer
		{
			KeyboardInitializer()
			{
				initKeyStates();
			}
		} _;

		void raise_callbacks(std::vector<std::function<void(Key)>> callbacks, user_input::Key key)
		{
			for (auto & callback : callbacks)
			{
				callback(key);
			}
		}
	}

	bool isKeyPressed(user_input::Key key)
	{
		return GetKeyState(static_cast<short>(key)) & 0x8000;
	}

	void registerHandlerFor_KeyUp(std::function<void(Key)> callback)
	{
		_keyUpCallbacks.emplace_back(callback);
	}

	void registerHandlerFor_KeyPressed(std::function<void(Key)> callback)
	{
		_keyPressedCallbacks.emplace_back(callback);
	}

	void registerHandlerFor_KeyDown(std::function<void(Key)> callback)
	{
		_keyDownCallbacks.emplace_back(callback);
	}

	void updateKeyboardState()
	{
		bool pressed = false;

		for (KeyState &keyState : _keyStateList)
		{
			pressed = isKeyPressed(keyState.getKey());

			if (pressed)
			{
				if (keyState.isPressed())
				{
					raise_callbacks(_keyPressedCallbacks, keyState.getKey());
				}
				else
				{
					raise_callbacks(_keyDownCallbacks, keyState.getKey());
				}
			}
			else
			{
				if (keyState.isPressed())
				{
					raise_callbacks(_keyUpCallbacks, keyState.getKey());
				}
			}

			keyState.setPressed(pressed);
		}
	}
}