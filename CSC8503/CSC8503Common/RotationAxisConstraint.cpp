#include "RotationAxisConstraint.h"
#include "PhysicsObject.h"
#include "GameObject.h"

using namespace NCL::CSC8503;

bool NCL::CSC8503::RotationAxisConstraint::UpdateConstraint(float dt)
{
	PhysicsObject* physOb = object->GetPhysicsObject();

	Vector3 velocity = physOb->GetAngularVelocity();
	physOb->SetLinearVelocity(velocity * axis);
	return true;

}