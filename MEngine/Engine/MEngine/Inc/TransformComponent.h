#pragma once

#include "Component.h"

namespace MEngine
{
	class TransformComponent final : public Component, public Graphics::Transform
	{
	public:
		SET_TYPE_ID(ComponentId::Transform);

		void DebugUI() override;
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(const rapidjson::Value& value) override;
	};
}