#pragma once

#include "Bone.h"

namespace MEngine::Graphics
{
	struct Skeleton
	{
		Bone* root = nullptr;
		std::vector<std::shared_ptr<Bone>> bones;

	};
}