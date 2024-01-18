#include "Precompiled.h"
#include "App.h"

using namespace MEngine;
using namespace MEngine::Core;

void App::Run(float runTime)
{
    LOG("App Started: %.3f", TimeUtil::GetTime());
    mRunning = true;
    while (mRunning)
    {
        float deltaTime = TimeUtil::GetDeltaTime();
        LOG("DeltaTime: %.3f", deltatime);
        runTime -= deltaTime;
        if (runTime <= 0.0f)
        {
            Quit();
        }
        //do stuff in game
    }
}

void App::Quit()
{
    mRunning = false;
}
