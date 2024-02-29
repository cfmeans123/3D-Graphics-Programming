#include "Precompiled.h"
#include "DebugUI.h"

#include "GraphicsSystem.h"
#include <ImGui/Inc/imgui_impl_dx11.h>
#include <ImGui/Inc/imgui_impl_win32.h>

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Core;

void DebugUI::StaticInitialize(HWND window, bool docking, bool multiViewport)
{
}

void DebugUI::StaticTerminate()
{
}