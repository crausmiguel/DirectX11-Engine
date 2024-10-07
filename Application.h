#pragma once

#include "DirectX.h"

//GLOBALS
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class Application {
public:
	Application();
	Application(const Application& src);
	~Application();

	bool Initialize(int height, int width, HWND hwnd);
	void Shutdown();
	bool DrawFrame();

private:
	DirectX* m_Direct3D;
	bool Render();

};
