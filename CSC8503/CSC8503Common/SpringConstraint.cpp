#include "SpringConstraint.h"
#include "PhysicsObject.h"
#include "GameObject.h"

bool NCL::CSC8503::SpringConstraint::UpdateConstraint(float dt)
{
	PhysicsObject* phys1 = object1->GetPhysicsObject();
	PhysicsObject* phys2 = object2->GetPhysicsObject();
	float stretch = ((object1->GetTransform().GetPosition() - object2->GetTransform().GetPosition()).Length()) - restingLength;//if negative then contracted
	Vector3 springDirection = ((object1->GetTransform().GetPosition() - object2->GetTransform().GetPosition()).Normalised());
	float force = (-1 * snappiness * stretch);
	float startDamp = damping * (Vector3::Dot(springDirection, phys1->GetLinearVelocity()));
	float endDamp = damping * (Vector3::Dot(springDirection, phys2->GetLinearVelocity()));
	phys1->AddForce(springDirection * (force - startDamp));
	phys2->AddForce(springDirection * (force - endDamp) * -1);
	return true;

}
