#include "MEngine/Inc/MEngine.h"
#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;



int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	App& myApp = MEngine::MainApp();
	myApp.AddState<GameState>("Game");
	myApp.AddState<CubeState>("CubeState");
	myApp.AddState<RectState>("RectState");
	myApp.AddState<PlaneState>("PlaneState");
	myApp.AddState<SphereState>("SphereState");
	myApp.AddState<CylinderState>("CylinderState");
	myApp.AddState<SkyboxState>("SkyboxState");
	myApp.AddState<SkysphereState>("SkysphereState");


	AppConfig config;
	config.appName = L"Hello Mesh Builder";

	myApp.Run(config);
	return(0);
}