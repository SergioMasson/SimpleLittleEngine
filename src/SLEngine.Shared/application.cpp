#include "pch.h"
#include "assert.h"
#include "application.h"
#include "graphics/coreGraphics.h"
#include "audio/audioCore.h"
#include "physics/physicsCore.h"
#include "input.h"
#include "systemTime.h"
#include <iostream>
#include <string>
#include "graphics/PostProcessing/postProcessingEffects.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

LONG defaultWidth = 1280;
LONG defaultHeight = 720;

int64_t previewsFrameTick = 0;
int64_t initialTick = 0;

using namespace sle;

HWND sle::g_coreWindow;

IGameApp* g_currentApp;

void InitializeApplication(IGameApp& app)
{
	graphics::Initialize(defaultWidth, defaultHeight);
	audio::Initialize();
	time::Initialize();
	Input::Initialize();
	physics::Initialize();

	app.Startup();
}

bool UpdateApplication(IGameApp& app)
{
	auto currentTick = time::GetCurrentTick();
	float deltaTime = static_cast<float>(time::TimeBetweenTicks(previewsFrameTick, currentTick));
	float totalTIme = static_cast<float>(time::TimeBetweenTicks(initialTick, currentTick));

	previewsFrameTick = currentTick;

	//Update all user inputs.
	Input::Update(deltaTime);

	//Update things before physics.
	app.FixedUpdate(deltaTime, totalTIme);

	//Update physics calculations.
	physics::Update(deltaTime);

	//Update things after physics.
	app.Update(deltaTime, totalTIme);

	//Render scene.
	graphics::BeginDraw();
	app.RenderScene();

	graphics::postEffects::Render();

	app.RenderUI();
	graphics::Present();

	double fps = 1.0 / deltaTime;
	std::string titlebar = "FPS: " + std::to_string(fps);

	SetWindowTextA(g_coreWindow, titlebar.c_str());

	Input::PostUpdate();

	return !app.IsDone();
}

void TerminateApplication(IGameApp& app)
{
	physics::Shutdown();
}

//Creates the window and manages the lifetime of the application.
void sle::RunApplication(IGameApp& app, HINSTANCE instance, const wchar_t* className)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = (CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS);
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(instance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = className;
	wcex.hIconSm = LoadIcon(instance, IDI_APPLICATION);

	ASSERT(0 != RegisterClassEx(&wcex), "Unable to register a window");

	// Create window
	RECT rc = { 0, 0, defaultWidth, defaultHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	g_coreWindow = CreateWindow(className, className, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, instance, nullptr);

	assert(g_coreWindow != 0);

	g_currentApp = &app;

	InitializeApplication(app);

	ShowWindow(g_coreWindow, SW_SHOWDEFAULT);

	initialTick = time::GetCurrentTick();

	do
	{
		MSG msg = {};
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT || msg.message == WM_DESTROY || msg.message == WM_CLOSE)
			break;
	} while (UpdateApplication(app));    // Returns false to quit loop

	TerminateApplication(app);
}

// Called every time the application receives event from windows.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:		//This will be called on window resize. Both width and height will be on the lParam
		graphics::Resize((UINT)(UINT64)lParam & 0xFFFF, (UINT)(UINT64)lParam >> 16);
		g_currentApp->Resize((UINT)(UINT64)lParam & 0xFFFF, (UINT)(UINT64)lParam >> 16);
		break;

		//This will be called on key first press.
	case WM_KEYDOWN:
		Input::SetKey(wParam, true);
		break;

	case WM_MOUSEWHEEL:
		Input::SetMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		break;

	case WM_KEYUP:
		Input::SetKey(wParam, false);
		break;

		//This will be called every frame when the key is pressed.
	case WM_CHAR:
		Input::SetTypedChar(wParam);
		break;

	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}