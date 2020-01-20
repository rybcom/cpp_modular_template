#include "Keyboard.h"
#include "KeyState.h"
#include <vector>
#include <chrono>

namespace user_input
{

		using KeyStateList = std::vector<KeyState>;

		std::wstring _windowHandlerName;
		bool _checkForWindow = false;
		KeyStateList _keyStateList;

		std::vector<std::function<void(Key, DeltaTime)>> _keyPressedCallbacks;
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

		bool isHandlerWindowActive()
		{
			auto wstring_to_string = [](std::wstring const& s)
			{
				std::string temp(s.length(), ' ');
				std::copy(s.begin(), s.end(), temp.begin());
				return temp;
			};

			if (_checkForWindow)
			{
				auto hwnd = GetActiveWindow();
				if (hwnd)
				{
					int len = GetWindowTextLength(hwnd);
					std::wstring s;
					s.resize(len);
					GetWindowText(hwnd, const_cast<wchar_t*>(s.c_str()), len );

					return s == _windowHandlerName;

				}
				return false;

			}
			else
			{
				return true;
			}
		}

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
			if (isHandlerWindowActive())
			{
				for (auto& callback : callbacks)
				{
					callback(key);
				}
			}
		}

		void raise_callbacks(std::vector<std::function<void(Key, DeltaTime)>> callbacks, user_input::Key key, DeltaTime deltaTime)
		{
			if (isHandlerWindowActive())
			{
				for (auto& callback : callbacks)
				{
					callback(key, deltaTime);
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

	void registerHandlerFor_KeyPressed(std::function<void(Key, DeltaTime)> callback)
	{
		_keyPressedCallbacks.emplace_back(callback);
	}

	void registerHandlerFor_KeyDown(std::function<void(Key)> callback)
	{
		_keyDownCallbacks.emplace_back(callback);
	}

	void updateKeyboardState()
	{
		using Clock = std::chrono::high_resolution_clock;
		auto static timepoint_1 = Clock::now();
		auto timepoint_2 = Clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(timepoint_2 - timepoint_1).count();


		bool pressed = false;

		for (KeyState& keyState : _keyStateList)
		{
			pressed = isKeyPressed(keyState.getKey());

			if (pressed)
			{
				if (keyState.isPressed())
				{
					raise_callbacks(_keyPressedCallbacks, keyState.getKey(), deltaTime / 1000000.0);
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

		timepoint_1 = timepoint_2;
	}


	void setWindowNameHandler(std::wstring const& name)
	{
		_checkForWindow = true;
		_windowHandlerName = name;
	}

}