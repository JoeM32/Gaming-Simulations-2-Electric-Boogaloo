#include "RotationConstraint.h"
#include "PhysicsObject.h"
#include "GameObject.h"

using namespace NCL::CSC8503;

float Dot(const Vector3& a, const Vector3& b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

bool RotationConstraint::UpdateConstraint(float dt) {

	PhysicsObject* physA = objectA->GetPhysicsObject();
	PhysicsObject* physB = objectB->GetPhysicsObject();
	float alignment = Dot(objectA->GetTransform().GetOrientation() * Vector3(0,1,0), objectB->GetTransform().GetOrientation() * Vector3(0, 1, 0));

	Vector3 average = (objectA->GetTransform().GetOrientation().ToEuler() + objectB->GetTransform().GetOrientation().ToEuler()) / 2.0f;
	
	//std::cout << objectA->GetTransform().GetOrientation().ToEuler() << objectB->GetTransform().GetOrientation().ToEuler() << "\n";


	if (abs(alignment) > rotation)
	{

		Vector3 correction = objectA->GetTransform().GetOrientation().ToEuler() - average;
		float biasFactor = 0.01f;
		float bias = (biasFactor / dt) * abs(alignment);

		float constraintMass = physA->GetInverseMass() +
			physB->GetInverseMass();

		Vector3 correctionImpulse = (correction * bias) / constraintMass;

		physA->ApplyAngularImpulse(correctionImpulse);
		physB->ApplyAngularImpulse(-correctionImpulse);
	}
	return true;
}