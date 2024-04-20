#pragma once
#include "Colors.h"

namespace MEngine::Graphics
{
    struct Material
    {
        Color ambient = Colors::White;
        Color diffuse = Colors::White;
        Color specular = Colors::White;
        Color emissive = Colors::Black;
        float power = 10.0f;
        float padding[3] = { 0.0f };
    };

}
