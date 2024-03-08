#include <MEngine/Inc/MEngine.h>

#include "GameState.h"

using namespace MEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	App& myApp = MainApp();
	myApp.AddState<MilkyWay>("MilkyWay");

	AppConfig config;
	config.appName = L"This Shape";

	myApp.Run(config);

	return(0);
}