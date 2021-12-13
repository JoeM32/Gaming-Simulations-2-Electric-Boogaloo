#include "SpinConstraint.h"
#include "PhysicsObject.h"
#include "GameObject.h"

using namespace NCL::CSC8503;

bool NCL::CSC8503::SpinConstraint::UpdateConstraint(float dt)
{
	PhysicsObject* physOb = object->GetPhysicsObject();
	physOb->SetAngularVelocity(axis * speed);
	return true;
}
