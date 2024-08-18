#include "MEngine/Inc/MEngine.h"
#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;



int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	App& myApp = MEngine::MainApp();
	myApp.AddState<GameState>("Game");
	

	AppConfig config;
	config.appName = L"Hello Particles";

	myApp.Run(config);
	return(0);
}