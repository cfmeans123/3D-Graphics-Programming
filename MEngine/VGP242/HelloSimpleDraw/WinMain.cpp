#include "MEngine/Inc/MEngine.h"
#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;



int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	App& myApp = MEngine::MainApp();
	myApp.AddState<GameState>("Game");
	myApp.AddState<TransformState>("TransformState");
	myApp.AddState<SphereState>("SphereState");
	myApp.AddState<AABBState>("AABBState");
	myApp.AddState<AABBFilledState>("AABBFilledState");
	myApp.AddState<LinesState>("LinesState");

	AppConfig config;
	config.appName = L"Hello Simple Draw";

	myApp.Run(config);
	return(0);
}