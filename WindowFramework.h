#pragma once
#include "WinSetup.h"
#include "Input.h"
#include "Application.h"

class WindowFramework {
public:
	WindowFramework();
	WindowFramework(const WindowFramework& src);
	~WindowFramework();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool DrawFrame();
	void InitializeWindows(int& width, int& height);
	void ShutdownWindows();

private:
	LPCWSTR m_ApplicationName;
	HINSTANCE m_hInstance;
	HWND m_Hwnd;

	Input* m_Input = { 0 };
	Application* m_Application = { 0 };

};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static WindowFramework* ApplicationHandle = 0;