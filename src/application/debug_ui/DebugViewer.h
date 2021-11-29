#pragma once
#include "project_config.h"

namespace debug_ui
{
	enum class DebugViewerResult
	{
		OK,
		Error,
		RequestClose
	};

	void init();

	DebugViewerResult update();

	void close();
}
