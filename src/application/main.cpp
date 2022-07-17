

#include "Utilities.h"
#include <thread>
#include <condition_variable>
#include <mutex>
#include "resources/resource.h"

#include <Windows.h>
#include <StopWatch.h>
#include "Application.h"
#include "configs/windowconfig.h"

namespace
{
	bool app_exit_request{ false };
	std::mutex m;
	std::condition_variable cv;
	bool closing_sequence_is_done = false;

	void application_request_to_close()
	{
		app_exit_request = true;
	}

	void notify_closing_thread()
	{
		{
			std::lock_guard<std::mutex> lk(m);
			closing_sequence_is_done = true;
		}
		cv.notify_one();
	}

	void closing_thread_wait()
	{
		std::unique_lock<std::mutex> lk(m);

		application_request_to_close();
		cv.wait(lk, [] {return closing_sequence_is_done; });
	}

	BOOL WINAPI request_close(DWORD dwType)
	{
		closing_thread_wait();
		return TRUE;
	}

	void init_console()
	{
		::ShowWindow(::GetConsoleWindow(),
			config::windows.is_console_visible ? SW_SHOW : SW_HIDE);

		std::wstring console_title = aux::string_to_wstring(aux::get_command_parameter(0).value_or("default app") + " - console");
		::SetConsoleTitle(console_title.c_str());

		SetConsoleCtrlHandler(request_close, TRUE);

		events::request_close_application.bind(application_request_to_close);
	}

	void do_main_loop_execution()
	{

		BEGIN_PROFILER_SESSION("profiler_stats.json");

		SandboxApp& app = SandboxApp::get();
		app.init();

		StopWatch watch;
		watch.Restart();

		const uint64_t time_dilatation_us = 16666.6667;

		while (app_exit_request == false)
		{
			auto const elapsed_time_us = watch.elapsed_us();

			if (elapsed_time_us > time_dilatation_us)
			{
				watch.Restart();
				app.update();
			}
		}

		app.close();

		END_PROFILER_SESSION();
	}
}

int main()
{
	init_console();

	do_main_loop_execution();

	notify_closing_thread();

	return 0;
}
