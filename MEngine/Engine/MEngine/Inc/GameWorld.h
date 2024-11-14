#pragma once

#include "Service.h"
#include "GameObject.h"

namespace MEngine
{
	using CustomService = std::function<Service* (const std::string&, GameWorld&)>;

	class GameWorld final
	{
	public:
		static void SetCustomService(CustomService customService);

		void Initialize(uint32_t capacity = 10);
		void Terminate();
		void Update(float deltaTime);
		void Render();
		void DebugUI();

		void LoadLevel(const std::filesystem::path& levelFile);
		void SaveLevel(std::filesystem::path saveFile = "");

		GameObject* CreateGameObject(std::string name, const std::filesystem::path& templatePath = "", bool initialize = false);
		void DestroyGameObject(const GameObjectHandle& handle);

		template<class ServiceType>
		ServiceType* AddService()
		{
			static_assert(std::is_base_of_v<Service, ServiceType>,
				"GameWorld: service must be of type Service");
			ASSERT(!mInitialized, "GameWorld: cant add service after initialized");

			auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
			newService->mWorld = this;
			return static_cast<ServiceType*>(newService.get());
		}

		template<class ServiceType>
		const ServiceType* GetService() const
		{
			for (auto& service : mServices)
			{
				if (service->GetTypeId() == ServiceType::StaticGetTypeId())
				{
					return static_cast<ServiceType*>(service.get());
				}
			}
			return nullptr;
		}

		template<class ServiceType>
		ServiceType* GetService()
		{
			const GameWorld* thisConst = static_cast<const GameWorld*>(this);
			return const_cast<ServiceType*>(thisConst->GetService<ServiceType>());
		}

	private:
		bool IsValid(const GameObjectHandle& handle);
		void ProcessDestroyList();

		struct Slot
		{
			std::unique_ptr<GameObject> gameObject;
			uint32_t generation = 0;
		};

		using GameObjectSlots = std::vector<Slot>;
		GameObjectSlots mGameObjectSlots;
		std::vector<uint32_t> mFreeSlots;
		std::vector<uint32_t> mToBeDestroyed;

		using Services = std::vector<std::unique_ptr<Service>>;
		Services mServices;

		std::filesystem::path mlevelFileName;
		bool mInitialized = false;
	};
}