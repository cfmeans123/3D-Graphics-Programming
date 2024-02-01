#include "Precompiled.h"
#include "MEngine.h"

MEngine::App& MEngine::MainApp()
{
	static App sApp;
	return sApp;
}