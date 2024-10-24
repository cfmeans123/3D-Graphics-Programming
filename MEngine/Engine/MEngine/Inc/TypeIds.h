#pragma once

namespace MEngine
{
	enum class ComponentId
	{
		Invalid,	//Default value
		Transform,	//trnasform component for location data
		Camera,		// contains a Camera for viewing
		FPSCamera,  // moves Camera with FPS controls
		Mesh,		// create a mesh render component
		Count
	};

	enum class ServiceId
	{
		Invalid,
		Camera,
		Count,
		Render
	};
}

#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId() {return static_cast<uint32_t>(id);}\
	uint32_t GetTypeId() const override {return StaticGetTypeId(); }