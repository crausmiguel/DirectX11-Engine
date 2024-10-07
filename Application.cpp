#include "Application.h"


Application::Application(){
	m_Direct3D = 0;
}
Application::Application(const Application& src){}
Application::~Application(){}


bool Application::Initialize(int screenWidth, int screenHeight, HWND hwnd){
	bool result;

	m_Direct3D = new Direct3D;
	
	result = m_Direct3D->Initialize(screenWidth, screenHeight, hwnd,
		VSYNC_ENABLED, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct 3D", L"Error", MB_OK);
		return false;
	}
	
	return true;
}


void Application::Shutdown(){
	if (m_Direct3D) {
		m_Direct3D->Shutdow();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

}


bool Application::DrawFrame(){
	bool result;

	result = Render();
	if (!result) {
		return false;
	}

	return true;
}


bool Application::Render(){
	m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	m_Direct3D->EndScene();

	return true;
}