#pragma once
#include "KeyboardEnums.h"
#include <functional>

namespace user_input
{

	bool isKeyPressed(Key key);

	void updateKeyboardState();

	void registerHandlerFor_KeyUp(std::function<void(Key)>);

	void registerHandlerFor_KeyPressed(std::function<void(Key)>);

	void registerHandlerFor_KeyDown(std::function<void(Key)>);

}