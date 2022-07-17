#include "project_configuration.h"
#if USE_GUI()

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "configs/WindowConfig.h"
#include "resources/resource.h"
#include <thread>
#include <mutex>
#include "DebugViewer.h"
#include "Utilities.h"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace GUI_Impl
{
	void ShowGUI();
	void LogText(LogVerbosity category, std::string_view text);
	void SaveCurrnetUILayout();
	void LoadUILayout(std::string_view layoutName);
}

namespace debug_ui
{
	static bool request_close_viewer = false;
	static bool critical_error_occured = false;
	static std::mutex exec_token;
	// Data
	static ID3D11Device* g_pd3dDevice = NULL;
	static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	static IDXGISwapChain* g_pSwapChain = NULL;
	static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

	// Forward declarations of helper functions
	bool CreateDeviceD3D(HWND hWnd, unsigned int fps);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static bool gui_window_is_minimalized = false;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	MSG msg;
	bool show_demo_window = true;
	HWND hwnd;
	WNDCLASSEX wc;

	void init()
	{

		log_notification::events::LogEvent.bind(GUI_Impl::LogText);

		std::wstring window_title = aux::string_to_wstring(aux::get_command_parameter(0).value_or("application") + " - viewer");

		// Create application window
		wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("SandboxApp"), NULL };
		::RegisterClassEx(&wc);
		hwnd = ::CreateWindow(wc.lpszClassName, window_title.c_str(), WS_OVERLAPPEDWINDOW,
			config::windows.viewer_position_x,
			config::windows.viewer_position_y,
			config::windows.viewer_width,
			config::windows.viewer_height, NULL, NULL, wc.hInstance, NULL);

		// Initialize Direct3D
		if (!CreateDeviceD3D(hwnd, 60))
		{
			CleanupDeviceD3D();
			::UnregisterClass(wc.lpszClassName, wc.hInstance);
			return;
		}

		if (config::windows.is_debug_viewer_visible)
		{
			::ShowWindow(hwnd,
				config::windows.is_debug_viewer_maximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
		}
		else
		{
			::ShowWindow(hwnd, SW_HIDE);
		}
		::UpdateWindow(hwnd);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();
		ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Read 'misc/fonts/README.txt' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 20.0f);
		//io.Fonts->AddFontFromFileTTF("fonts/Cousine-Regular.ttf", 20.0f);
		//io.Fonts->AddFontFromFileTTF("fonts/DroidSans.ttf", 20.0f);
		//io.Fonts->AddFontFromFileTTF("fonts/Karla-Regular.ttf", 16.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != NULL);

		ZeroMemory(&msg, sizeof(msg));

		//GUI_Impl::LoadUILayout();
	}

	DebugViewerResult update()
	{
		if (request_close_viewer == true)
		{
			return DebugViewerResult::RequestClose;
		}


		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while (msg.message != WM_QUIT)
		{
			// Poll and handle messages (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
				continue;
			}


			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			std::lock_guard lock_execution(exec_token);
			try {
				GUI_Impl::ShowGUI();
			}
			catch (const std::overflow_error& e) {
				CRITICAL_LOG("GUI overflow error {}", e.what());
				critical_error_occured = true;
			}
			catch (const std::runtime_error& e) {
				CRITICAL_LOG("GUI runtime error {}", e.what());
				critical_error_occured = true;
			}
			catch (const std::exception& e) {
				CRITICAL_LOG("GIU exception {}", e.what());
				critical_error_occured = true;
			}
			catch (...) {
				CRITICAL_LOG("GUI non-defined exception {}");
				critical_error_occured = true;
			}

			// Rendering
			ImGui::Render();
			g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
			g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			//g_pSwapChain->Present(1, 0); // Present with vsync
			g_pSwapChain->Present(0, 0); // Present without vsync

			if (critical_error_occured == true)
			{
				return DebugViewerResult::Error;
			}
			else if (request_close_viewer == true)
			{
				return DebugViewerResult::RequestClose;
			}
			else
			{
				return DebugViewerResult::OK;
			}
		}
	}

	void close()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		CleanupDeviceD3D();
		::DestroyWindow(hwnd);
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
	}

	// Helper functions

	bool CreateDeviceD3D(HWND hWnd, unsigned int fps)
	{
		// Setup swap chain
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = fps;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		UINT createDeviceFlags = 0;
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
			return false;

		CreateRenderTarget();
		return true;
	}

	void CleanupDeviceD3D()
	{
		CleanupRenderTarget();
		if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
		if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
		if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	}

	void CreateRenderTarget()
	{
		ID3D11Texture2D* pBackBuffer;
		g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
		pBackBuffer->Release();
	}

	void CleanupRenderTarget()
	{
		if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
	}

	// Win32 message handler
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		switch (msg)
		{
		case WM_SIZE:
			if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
			{
				CleanupRenderTarget();
				g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
				CreateRenderTarget();
			}
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;

			else if ((wParam & 0xfff0) == SC_MINIMIZE)
			{
				gui_window_is_minimalized = true;
			}
			else if ((wParam & 0xfff0) == SC_RESTORE)
			{
				gui_window_is_minimalized = false;
			}
			break;
		case WM_DESTROY:
			request_close_viewer = true;
			return 0;
		}
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

}

#endif