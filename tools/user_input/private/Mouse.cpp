#include "Mouse.h"
#include <vector>
#include <Windows.h>
#include "MouseButtonState.h"
#include "mouseHook.h"
#include <iostream>

namespace user_input
{


	std::string toString(MouseButton const & button)
	{
		switch (button)
		{
		case MouseButton::Cancel: return "MouseButton:Cancel";
		case MouseButton::Left: return "MouseButton:Left";
		case MouseButton::Right: return "MouseButton:Right";
		case MouseButton::Middle: return "MouseButton:Middle";
		case MouseButton::toBack: return "MouseButton:toBack";
		case MouseButton::toForward: return "MouseButton:toForward";
		default:
			return "not defined mouse button";	
		}
	}

	namespace
	{
		RECT actualDesktop;
		

		std::vector<std::function<void(MouseMoveArgs const &)>> _mouseMoveCallbacks;
		std::vector<std::function<void(MouseButton const &)>> _mouseButtonPressedCallbacks;
		std::vector<std::function<void(MouseButton const &)>> _mouseButtonDownCallbacks;
		std::vector<std::function<void(MouseButton const &)>> _mouseButtonUpCallbacks;
		std::vector<std::function<void(int)>> _mouseScrollCallbacks;

		using MouseButtonList = std::vector<MouseButtonState>;
		MouseButtonList _mouseButtonList;
		MouseButton const MouseButtons[] = {
			MouseButton::Left,
			MouseButton::Right,
			MouseButton::Cancel,
			MouseButton::Middle,
			MouseButton::toBack,
			MouseButton::toForward,
		};

		void initMouseState()
		{
			for (MouseButton button : MouseButtons)
			{
				_mouseButtonList.push_back(MouseButtonState(button));
			}
		}

		struct MousePositionState
		{
			CursorPosition currentPosition;
			CursorPosition oldPosition;
		} _mousePositionState;

		struct MouseInitializer
		{
			MouseInitializer()
			{
				initMouseState();
			//	initMouseHook();
				GetWindowRect(GetDesktopWindow(), &actualDesktop);
			}
			
			~MouseInitializer()
			{
			//	releaseMouseHook();
			}

		} _;

		void raise_mouse_move_callbacks(std::vector<std::function<void(MouseMoveArgs const &)>> callbacks, MouseMoveArgs const & args)
		{
			for (auto & callback : callbacks)
			{
				callback(args);
			}
		}

		void raise_callbacks(std::vector<std::function<void(MouseButton const &)>> callbacks, MouseButton const & button)
		{
			for (auto & callback : callbacks)
			{
				callback(button);
			}
		}

		void raise_callbacks(std::vector<std::function<void(int)>> callbacks, int value)
		{
			for (auto & callback : callbacks)
			{
				callback(value);
			}
		}

		MouseMoveArgs get_mouse_move_args()
		{
			MouseMoveArgs args;
			args.newPosition = _mousePositionState.currentPosition;
			args.oldPosition = _mousePositionState.oldPosition;
			args.diffX = _mousePositionState.oldPosition.x - _mousePositionState.currentPosition.x;
			args.diffY = _mousePositionState.oldPosition.y - _mousePositionState.currentPosition.y;

			return args;
		}

		void updateMousekeys()
		{
			bool pressed = false;

			for (MouseButtonState &buttonState : _mouseButtonList)
			{
				pressed = isMouseButtonPressed(buttonState.getButton());

				if (pressed)
				{
					if (buttonState.isPressed())
					{
						raise_callbacks(_mouseButtonPressedCallbacks, buttonState.getButton());
					}
					else
					{
						raise_callbacks(_mouseButtonDownCallbacks, buttonState.getButton());
					}
				}
				else
				{
					if (buttonState.isPressed())
					{
						raise_callbacks(_mouseButtonUpCallbacks, buttonState.getButton());
					}
				}

				buttonState.setPressed(pressed);
			}
		}

		void do_cursor_monitor_bounding_check(POINT & p)
		{
   			if (p.x == actualDesktop.left)
			{
				p.x = actualDesktop.right;
				GetWindowRect(GetDesktopWindow(), &actualDesktop);
				SetCursorPos(p.x, p.y);
				_mousePositionState.currentPosition.x = p.x + 1;
			}
			else if (p.x == actualDesktop.right - 1)
			{
				p.x = actualDesktop.left;
				GetWindowRect(GetDesktopWindow(), &actualDesktop);
				SetCursorPos(p.x, p.y);
				_mousePositionState.currentPosition.x = p.x - 1;
			}

			if (p.y == actualDesktop.top)
			{
				p.y = actualDesktop.bottom;
				GetWindowRect(GetDesktopWindow(), &actualDesktop);
				SetCursorPos(p.x, p.y);
				_mousePositionState.currentPosition.y = p.y + 1;
			}
			else if (p.y == actualDesktop.bottom-1)
			{
 				p.y = actualDesktop.top;
				GetWindowRect(GetDesktopWindow(), &actualDesktop);
				SetCursorPos(p.x, p.y);
				_mousePositionState.currentPosition.y = p.y - 1;
			}
			
		}

		void updateMousePosition()
		{
			static POINT p;
			GetCursorPos(&p);
		//	do_cursor_monitor_bounding_check(p);

			CursorPosition cursorPosition = { p.x,p.y };

			if (cursorPosition != _mousePositionState.currentPosition)
			{
				_mousePositionState.oldPosition = _mousePositionState.currentPosition;
				_mousePositionState.currentPosition = cursorPosition;
				
				MouseMoveArgs args = get_mouse_move_args();
				raise_mouse_move_callbacks(_mouseMoveCallbacks, args);
			}
		}
	}

	void process_scrolling(int scrolValue)
	{
		raise_callbacks(_mouseScrollCallbacks, scrolValue);
	}

	void registerHandlerFor_MouseMove(std::function<void(MouseMoveArgs const &)> callback)
	{
		_mouseMoveCallbacks.emplace_back(callback);
	}

	void registerHandlerFor_MouseButtonPressed(std::function<void(MouseButton const &)> callback)
	{
		_mouseButtonPressedCallbacks.emplace_back(callback);
	}

	void registerHandlerFor_MouseButtonUp(std::function<void(MouseButton const &)> callback)
	{
		_mouseButtonUpCallbacks.emplace_back(callback);
	}

	void registerHandlerFor_MouseButtonDown(std::function<void(MouseButton const &)> callback)
	{
		_mouseButtonDownCallbacks.emplace_back(callback);
	}

	void registerHandlerFor_MouseScroll(std::function<void(int)> callback)
	{
		_mouseScrollCallbacks.emplace_back(callback);
	}

	void mouseStateUpdate()
	{
		updateMousePosition();

		updateMousekeys();

		//updateMouseScroll();
	}

	bool isMouseButtonPressed(MouseButton const & button)
	{
		return ((GetKeyState(static_cast<short>(button)) & 0x80) != 0);
	}

}