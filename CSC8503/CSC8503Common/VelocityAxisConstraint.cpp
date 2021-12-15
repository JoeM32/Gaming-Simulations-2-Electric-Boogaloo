#include "VelocityAxisConstraint.h"
#include "PhysicsObject.h"
#include "GameObject.h"

using namespace NCL::CSC8503;

bool NCL::CSC8503::VelocityAxisConstraint::UpdateConstraint(float dt)
{
	PhysicsObject* physOb = object->GetPhysicsObject();

	Vector3 velocity = physOb->GetLinearVelocity();
	physOb->SetLinearVelocity(axis.Normalised()*(Vector3::Dot(velocity,axis)/axis.Length()));
	return true;
}