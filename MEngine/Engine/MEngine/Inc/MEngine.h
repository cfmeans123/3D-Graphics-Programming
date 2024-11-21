#pragma once

#include "Common.h"

#include "App.h"
#include "AppState.h"

#include "Event.h"
#include "EventManager.h"

#include "SaveUtil.h"
#include "GameObjectFactory.h"

// Game World
#include "GameWorld.h"

// Services
#include "Service.h"
#include "CameraService.h"
#include "PhysicsService.h"
#include "RenderService.h"
#include "UIRenderService.h"

// Game Object
#include "GameObject.h"

// Components
#include "TypeIds.h"
#include "Component.h"
#include "AnimatorComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "RenderObjectComponent.h"
#include "RigidBodyComponent.h"
#include "SoundEffectComponent.h"
#include "SoundBankComponent.h"
#include "TransformComponent.h"
#include "UIComponent.h"
#include "UISpriteComponent.h"
#include "UITextComponent.h"

namespace MEngine
{
    App& MainApp();
}

