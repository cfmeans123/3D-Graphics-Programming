#pragma once


namespace MEngine::Physics
{
	class PhysicsObject
	{
	public:
		PhysicsObject() = default;
		virtual ~PhysicsObject() = default;

	protected:
		friend class PhysicsWorld;
		virtual void SyncGraphics() = 0;
		virtual btRigidBody* GetRigidBody() { return nullptr; }
		virtual btSoftBody* GetSoftBody() { return nullptr; }
	};
}