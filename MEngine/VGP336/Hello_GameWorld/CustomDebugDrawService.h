#pragma once

#include "CustomTypeIds.h"

class CustomDebugDrawComponent;

class CustomDebugDrawService : public MEngine::Service
{
public:
    SET_TYPE_ID(CustomServiceId::CustomDebugDrawDisplay);

    void Render() override;

    void Register(CustomDebugDrawComponent* debugDrawComponent);
    void Unregister(CustomDebugDrawComponent* debugDrawComponent);
private:
    using CustomDebugDrawComponents = std::vector<CustomDebugDrawComponent*>;
    CustomDebugDrawComponents mCustomDebugDrawComponents;
};