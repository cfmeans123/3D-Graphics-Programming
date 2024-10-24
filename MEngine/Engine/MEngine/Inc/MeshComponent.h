#pragma once

#include "RenderObjectComponent.h"

namespace MEngine
{
	class MeshComponent final : public RenderObjectComponent
	{
	public:
		SET_TYPE_ID(ComponentId::Invalid);

		void Deserialize(const rapidjson::Value& value) override;
		const Graphics::Model& GetModel() const override;
	
	private:
		Graphics::Model mModel;
	};
}