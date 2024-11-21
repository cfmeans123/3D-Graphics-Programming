#pragma once

#include "Component.h"

namespace MEngine
{
    class UIComponent : public Component
    {
    public:
        virtual void Render() = 0;
    };
}