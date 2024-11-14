#pragma once

#include <MEngine/Inc/MEngine.h>

enum class CustomComponentId
{
    CustomDebugDraw = static_cast<uint32_t>(MEngine::ComponentId::Count)
};

enum class CustomServiceId
{
    CustomDebugDrawDisplay = static_cast<uint32_t>(MEngine::ServiceId::Count)
};