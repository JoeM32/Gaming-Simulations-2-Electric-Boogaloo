#include "HookConstraint.h"
#include "PhysicsObject.h"
#include "GameObject.h"

bool NCL::CSC8503::HookConstraint::UpdateConstraint(float dt)
{


	PhysicsObject* phys1 = object->GetPhysicsObject();
	float stretch = ((object->GetTransform().GetPosition() - point).Length()) - restingLength;//if negative then contracted
	Vector3 springDirection = ((object->GetTransform().GetPosition() - point).Normalised());
	float force = (-1 * snappiness * stretch);
	float startDamp = damping * (Vector3::Dot(springDirection, phys1->GetLinearVelocity()));
	phys1->AddForce(springDirection * (force - startDamp));

	timer -= dt;
	if (timer < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
