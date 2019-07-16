
#pragma once

namespace user_input
{
	enum class MouseButton
	{
		Left = 0x01,
		Right = 0x02,
		Cancel = 0x03,
		Middle = 0x04,
		toBack = 0x05,
		toForward = 0x06,
	};

	std::string toString(MouseButton const & button);

}
