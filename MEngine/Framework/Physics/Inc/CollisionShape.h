#pragma once

namespace MEngine::Physics
{
	class CollisionShape final
	{
	public:
		CollisionShape() = default;
		~CollisionShape();

		void InitializeSphere(float radius);
		void InitializeBox(const MEngine::Math::Vector3& halfExtents);
		void InitializeHull(const MEngine::Math::Vector3& halfExtents, const MEngine::Math::Vector3& origin);
		void InitializeEmpty();
		void Terminate();

	private:
		btCollisionShape* mCollisionShape = nullptr;

		friend class RigidBody;
		btCollisionShape* GetCollisionShape() const { return mCollisionShape; }
	};
}