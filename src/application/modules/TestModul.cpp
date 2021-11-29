#pragma once
#include "project_config.h"
#include "Application.h"
#include "ActionInvoker.h"
#include "configs/settings.h"

using namespace action_invoker;

class TestModul : public BaseModul
{
public:

	virtual void init() override
	{
		INFO_LOG(__FUNCSIG__);
		INFO_LOG("welcome : {}", setting_config.welcome_phrase);

		printApplicationTimeEverySecond();
	}

	virtual void update() override
	{
		_defferedActions.update(DeltaTime{ SandboxApp::get().getDeltaTime() });
	}

	virtual void close() override
	{
		INFO_LOG(__FUNCSIG__);
		INFO_LOG("application is closing : {}", setting_config.goodbye_phrase);
	}

private:

	void printApplicationTime()
	{
		INFO_LOG("global app.time : {} s", SandboxApp::get().getGlobalTime());
	}

	void printApplicationTimeEverySecond()
	{
		printApplicationTime();
		_defferedActions.addDeferredAction(
			 std::bind(&TestModul::printApplicationTimeEverySecond, this)
			, DeltaTime{ 1 });
	}

	size_t _counter{ 0 };
	ActionInvoker _defferedActions;
};

namespace
{
	struct static_inicializer
	{
		static_inicializer()
		{
			SandboxApp::get().registerModule<TestModul>();
		}
	}_;
}
