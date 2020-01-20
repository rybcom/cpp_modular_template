#include <Windows.h>
#include <iostream>

namespace user_input
{
	void process_scrolling(int scrolValue);

	HHOOK _hook;

	LRESULT __stdcall HookCallback(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code >= 0)
		{
			if (wParam == WM_MOUSEWHEEL)
			{
				MSLLHOOKSTRUCT pMouseStruct = *(MSLLHOOKSTRUCT *)lParam;

				int scrollValue = GET_WHEEL_DELTA_WPARAM(pMouseStruct.mouseData) / WHEEL_DELTA;
	
				process_scrolling(scrollValue);
			}
		}

		return CallNextHookEx(_hook, code, wParam, lParam);
	}

	void initMouseHook()
	{
		if (!(_hook = SetWindowsHookEx(WH_MOUSE_LL, HookCallback, NULL, 0)))
		{
			MessageBoxW(nullptr, L"Failed to hook!", L"Error", MB_ICONERROR);
		}
	}

	void releaseMouseHook()
	{
		UnhookWindowsHookEx(_hook);
	}

	void updateMouseScroll()
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

}