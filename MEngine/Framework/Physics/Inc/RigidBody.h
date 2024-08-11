#pragma once

#include "PhysicsObject.h"

namespace MEngine::Physics
{
	class CollisionShape;

	class RigidBody final : public PhysicsObject
	{
	public:
		RigidBody() = default;
		~RigidBody() override;

		void Initialize(MEngine::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass = 0.0f);
		void Terminate();

		void SetPosition(const MEngine::Math::Vector3& position);
		void SetVelocity(const MEngine::Math::Vector3& velocity);

		bool IsDynamic() const;

	private:
		void SyncGraphics() override;
		btRigidBody* GetRigidBody() override { return mRigidBody; }

		btRigidBody* mRigidBody = nullptr;
		btDefaultMotionState* mMotionState = nullptr;
		float mMass = 0.0f;

		MEngine::Graphics::Transform* mGraphicsTransform = nullptr;
	};
}