#include "PositionAxisConstraint.h"
#include "PhysicsObject.h"
#include "GameObject.h"

using namespace NCL::CSC8503;

bool NCL::CSC8503::PositionAxisConstraint::UpdateConstraint(float dt)
{
	PhysicsObject* physOb = object->GetPhysicsObject();

	Vector3 velocity = physOb->GetLinearVelocity();
	physOb->SetLinearVelocity(velocity * axis);
	return false;
}



