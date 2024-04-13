#include "Precompiled.h"
#include "RenderObject.h"


using namespace MEngine;
using namespace MEngine::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
}