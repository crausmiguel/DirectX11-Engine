#include "WindowFramework.h"

WindowFramework::WindowFramework() {}
WindowFramework::WindowFramework(const WindowFramework& original) {}
WindowFramework::~WindowFramework() {}

bool WindowFramework::Initialize() {
	int screenWidth = 0, screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	m_Application = new Application();
	m_Input = new Input();
	
	return m_Application->Initialize(screenWidth, screenHeight, m_Hwnd);
	
}

void WindowFramework::Shutdown() {
	if (m_Application) {
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}
	if (m_Input) {
		delete m_Input;
		m_Input = 0;
	}
	ShutdownWindows();
}

//Calls drawframe while doesn't get the quit msg
void WindowFramework::Run() {
	MSG msg = { 0 };
	bool done = false, result;

	while (!done) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {
			result = DrawFrame();
			done = !result;
		}
	}
}

//Call Application class drawing function
bool WindowFramework::DrawFrame() {
	return m_Application->DrawFrame();
}

//Handle the keyboard inputs
LRESULT CALLBACK WindowFramework::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg) {
	case WM_KEYDOWN: {
		m_Input->KeyDown((unsigned int)wparam);
	}
	case WM_KEYUP: {
		m_Input->KeyUp((unsigned int)wparam);
	}
	default: {
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

//Create and register Window Class
void WindowFramework::InitializeWindows(int& screenWith, int& screenHeight){
	WNDCLASSEX wc;
	int posX, posY;


	ApplicationHandle = this;

	m_hInstance = GetModuleHandle(nullptr);

	m_ApplicationName = L"Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_ApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);


	RegisterClassEx(&wc);

	if (FULL_SCREEN) {

		//TO DO
		//Find a way to do that without use the WINAPI settings, bc is buggy
	} else {
		screenWith = 800;
		screenHeight = 600;

		//Spawn on the middle screen;
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWith) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
	
	m_Hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU, posX, posY, screenWith, screenHeight,
		nullptr, nullptr, m_hInstance, nullptr);

	ShowWindow(m_Hwnd, SW_SHOW);
	SetForegroundWindow(m_Hwnd);
	SetFocus(m_Hwnd);
	ShowCursor(false);
}

void WindowFramework::ShutdownWindows() {
	ShowCursor(true);

	if (FULL_SCREEN) {
		//TO DO
	}

	DestroyWindow(m_Hwnd);
	m_Hwnd = nullptr;

	UnregisterClass(m_ApplicationName, m_hInstance);
	m_hInstance = nullptr;

	ApplicationHandle = NULL;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg) {
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE: {
		PostQuitMessage(0);
		return 0;
	}
	default: {
		return ApplicationHandle->MessageHandler(hwnd, umsg, wparam, lparam);
	}
	}
}