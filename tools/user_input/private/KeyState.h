#pragma once

#include "KeyboardEnums.h"

namespace user_input
{

	class KeyState
	{

#pragma region constructros && destructor

	public:

		KeyState::KeyState(Key key)
			: _key{ key },
			_pressed{ false }
		{
		}

		~KeyState() = default;

#pragma endregion

#pragma region api

	public:


		Key getKey() const { return _key; }

		void setPressed(bool pressed) { _pressed = pressed; }

		bool isPressed() const { return _pressed; }


#pragma endregion

#pragma region members

	private:

		Key _key;

		bool _pressed;

#pragma endregion

	};

}