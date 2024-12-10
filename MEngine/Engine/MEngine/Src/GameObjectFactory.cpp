#include "Precompiled.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "Component.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"
#include "SoundEffectComponent.h"
#include "SoundBankComponent.h"
#include "UITextComponent.h"
#include "UISpriteComponent.h"

using namespace MEngine;

namespace
{
	CustomMake TryMake;
	CustomGet TryGet;

	Component* AddComponent(const std::string& componentName, GameObject& gameObject)
	{
		Component* newComponent = nullptr;
		if (componentName == "TransformComponent")
		{
			newComponent = gameObject.AddComponent<TransformComponent>();
		}
		else if (componentName == "CameraComponent")
		{
			newComponent = gameObject.AddComponent<CameraComponent>();
		}
		else if (componentName == "FPSCameraComponent")
		{
			newComponent = gameObject.AddComponent<FPSCameraComponent>();
		}
		else if (componentName == "MeshComponent")
		{
			newComponent = gameObject.AddComponent<MeshComponent>();
		}
		else if (componentName == "ModelComponent")
		{
			newComponent = gameObject.AddComponent<ModelComponent>();
		}
		else if (componentName == "AnimatorComponent")
		{
			newComponent = gameObject.AddComponent<AnimatorComponent>();
		}
		else if (componentName == "RigidBodyComponent")
		{
			newComponent = gameObject.AddComponent<RigidBodyComponent>();
		}
		else if (componentName == "SoundEffectComponent")
		{
			newComponent = gameObject.AddComponent<SoundEffectComponent>();
		}
		else if (componentName == "SoundBankComponent")
		{
			newComponent = gameObject.AddComponent<SoundBankComponent>();
		}
		else if (componentName == "UITextComponent")
		{
			newComponent = gameObject.AddComponent<UITextComponent>();
		}
		else if (componentName == "UISpriteComponent")
		{
			newComponent = gameObject.AddComponent<UISpriteComponent>();
		}
		else
		{
			newComponent = TryMake(componentName, gameObject);
			ASSERT(false, "GameObjectFactory: unrecognized component %s", componentName.c_str());
		}

		return newComponent;
	}
}
Component* GetComponent(const std::string& componentName, GameObject& gameObject)
{
	Component* newComponent = nullptr;
	if (componentName == "TransformComponent")
	{
		newComponent = gameObject.GetComponent<TransformComponent>();
	}
	else if (componentName == "CameraComponent")
	{
		newComponent = gameObject.GetComponent<CameraComponent>();
	}
	else if (componentName == "FPSCameraComponent")
	{
		newComponent = gameObject.GetComponent<FPSCameraComponent>();
	}
	else if (componentName == "MeshComponent")
	{
		newComponent = gameObject.GetComponent<MeshComponent>();
	}
	else if (componentName == "ModelComponent")
	{
		newComponent = gameObject.GetComponent<ModelComponent>();
	}
	else if (componentName == "AnimatorComponent")
	{
		newComponent = gameObject.GetComponent<AnimatorComponent>();
	}
	else if (componentName == "RigidBodyComponent")
	{
		newComponent = gameObject.GetComponent<RigidBodyComponent>();
	}
	else if (componentName == "SoundEffectComponent")
	{
		newComponent = gameObject.GetComponent<SoundEffectComponent>();
	}
	else if (componentName == "SoundBankComponent")
	{
		newComponent = gameObject.GetComponent<SoundBankComponent>();
	}
	else if (componentName == "UITextComponent")
	{
		newComponent = gameObject.GetComponent<UITextComponent>();
	}
	else if (componentName == "UISpriteComponent")
	{
		newComponent = gameObject.GetComponent<UISpriteComponent>();
	}
	else
	{
		newComponent = TryGet(componentName, gameObject);
		ASSERT(false, "GameObjectFactory: unrecognized component %s", componentName.c_str());
	}

	return newComponent;
}

void MEngine::GameObjectFactory::SetCustomMake(CustomMake customMake)
{
	TryMake = customMake;
}

void MEngine::GameObjectFactory::SetCustomGet(CustomMake customGet)
{
	TryGet = customGet;
}

void GameObjectFactory::Make(const std::filesystem::path& templatePath, GameObject& gameObject)
{
	FILE* file = nullptr;
	auto err = fopen_s(&file, templatePath.u8string().c_str(), "r");
	ASSERT(err == 0, "GameObjectFactory: failed to open file %s", templatePath.u8string().c_str());

	char readBuffer[65536];
	rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
	fclose(file);

	rapidjson::Document doc;
	doc.ParseStream(readStream);
	auto components = doc["Components"].GetObj();
	for (auto& component : components)
	{
		Component* newComponent = AddComponent(component.name.GetString(), gameObject);
		if (newComponent != nullptr)
		{
			newComponent->Deserialize(component.value);
		}
	}
}
void GameObjectFactory::OverrideDeserialize(const rapidjson::Value& value, GameObject& gameObject)
{
	if (value.HasMember("Components"))
	{
		auto components = value["Components"].GetObj();
		for (auto& component : components)
		{
			Component* ownedComponent = GetComponent(component.name.GetString(), gameObject);
			if (ownedComponent != nullptr)
			{
				ownedComponent->Deserialize(component.value);
			}
		}
	}
}
