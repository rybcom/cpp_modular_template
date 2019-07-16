#pragma once

#include "MouseEnums.h"

namespace user_input
{

	class MouseButtonState
	{

#pragma region constructros && destructor

	public:

		explicit MouseButtonState::MouseButtonState(MouseButton button)
			: _button{ button },
			_pressed{ false }
		{
		}

		~MouseButtonState() = default;

#pragma endregion

#pragma region api

	public:


		MouseButton getButton() const { return _button; }

		void setPressed(bool pressed) { _pressed = pressed; }

		bool isPressed() const { return _pressed; }


#pragma endregion

#pragma region members

	private:

		MouseButton _button;

		bool _pressed;

#pragma endregion

	};

}