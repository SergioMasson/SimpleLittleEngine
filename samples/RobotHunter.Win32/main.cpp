#include "pch.h"
#include "application.h"
#include "RobotHunterApp.h"

INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	ASSERT_SUCCEEDED(CoInitialize(nullptr), "CoInitialize failed");
	IGameApp* app = new RobotHunterApp();
	RunApplication(*app, hInstance, L"VoxelMachina");
	delete app;
	return 0;
}