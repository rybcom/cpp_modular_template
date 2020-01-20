#pragma once
#include "KeyboardEnums.h"
#include <functional>
#include "MouseEnums.h"

namespace user_input
{
	struct CursorPosition
	{
		int x = 0;
		int y = 0;

		bool operator==(CursorPosition const & other)
		{
			return  (x == other.x) && (y == other.y);
		}
		bool operator!=(CursorPosition const & other)
		{
			return !(*this == other);
		}
	};

	struct MouseMoveArgs
	{
		CursorPosition newPosition;
		CursorPosition oldPosition;
		int diffX = 0;
		int diffY = 0;
	};

	void mouseStateUpdate();

	bool isMouseButtonPressed(MouseButton const & button);

	void registerHandlerFor_MouseMove(std::function<void(MouseMoveArgs const & )>);

	void registerHandlerFor_MouseButtonPressed(std::function<void(MouseButton const &)>);

	void registerHandlerFor_MouseButtonUp(std::function<void(MouseButton const &)>);

	void registerHandlerFor_MouseButtonDown(std::function<void(MouseButton const &)>);

	void registerHandlerFor_MouseScroll(std::function<void(int)>);

}