#include "MEngine/Inc/MEngine.h"
#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;



int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	App& myApp = MEngine::MainApp();
	myApp.AddState<GameState>("Final");
	

	AppConfig config;
	config.appName = L"Hello Final";

	myApp.Run(config);
	return(0);
}