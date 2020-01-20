#pragma once
#include "KeyboardEnums.h"
#include <functional>

namespace user_input
{

	using DeltaTime = float;

	bool isKeyPressed(Key key);

	void updateKeyboardState();

	void setWindowNameHandler(std::wstring const & name);

	void registerHandlerFor_KeyUp(std::function<void(Key)>);

	void registerHandlerFor_KeyPressed(std::function<void(Key,DeltaTime)>);

	void registerHandlerFor_KeyDown(std::function<void(Key)>);

}