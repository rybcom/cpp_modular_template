#pragma once
#include "project_config.h"
#include "EventSystem.h"
#include <vector>
#include <functional>

class BaseModul
{
public:

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void close() = 0;
};

class SandboxApp
{

public:

	static SandboxApp& get();

	void init();

	void update();

	void close();

	template<typename T>
	void registerModule()
	{
		_modules.push_back(std::make_unique<T>());
	}

	double getDeltaTime() const;

	double getGlobalTime() const;

private:

	double _deltaTime{ 0.0 };
	double _globalTime{ 0.0 };

	SandboxApp() = default;
	std::vector<std::unique_ptr<BaseModul>> _modules;

};



namespace events
{
	inline event_system::Signal request_close_application;
}
