#include "Precompiled.h"
#include "TimeUtil.h"

using namespace MEngine;
using namespace MEngine::Core;

float TimeUtil::GetTime()
{
    //static only called the first time you call this function, gets us a baseline time to calculate from
    static const auto startTime = std::chrono::high_resolution_clock::now();
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
    return milliseconds / 1000.0f;
}

float TimeUtil::GetDeltaTime()
{
    static auto lastCallTime = std::chrono::high_resolution_clock::now();
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastCallTime).count();
    lastCallTime = currentTime;
    return milliseconds / 1000.0f;
}