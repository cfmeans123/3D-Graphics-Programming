#include "..\..\Engine\MEngine\Inc\MEngine.h"
#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;



int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	App& myApp = MEngine::MainApp();
	myApp.AddState<TriangleState>("Triangle");
	myApp.AddState<SquareState>("Square");
	myApp.AddState<TriforceState>("Triforce");
	myApp.AddState<DiamondState>("Diamond");
	myApp.AddState<HeartState>("Heart");

	AppConfig config;
	config.appName = L"Hello Window";

	myApp.Run(config);
	return(0);
}