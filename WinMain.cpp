
#include "WindowFramework.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	WindowFramework* system = new WindowFramework();
	bool result;

	result = system->Initialize();
	if (result) {
		system->Run();
	}

	system->Shutdown();
	delete system;
	system = 0;

	return 0;
}