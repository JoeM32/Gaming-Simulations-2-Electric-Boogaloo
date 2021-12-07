#include "RotationConstraint.h"
#include "PhysicsObject.h"
#include "GameObject.h"

using namespace NCL::CSC8503;

void RotationConstraint::UpdateConstraint(float dt) {
	Vector3 relativeRot =
		objectA->GetTransform().GetOrientation().ToEuler() -
		objectB->GetTransform().GetOrientation().ToEuler();

	float currentAngle = relativeRot.Length();

	float offset = rotation - currentAngle;
	if (abs(offset) > 0.0f) {
		Vector3 offsetDir = relativeRot.Normalised();

		PhysicsObject* physA = objectA->GetPhysicsObject();
		PhysicsObject* physB = objectB->GetPhysicsObject();

		Vector3 relativeAngularVelocity = physA->GetAngularVelocity() -
			physB->GetAngularVelocity();

		float constraintMass = physA->GetInverseMass() +
			physB->GetInverseMass();

		if (constraintMass > 0.0f) {
			//how much of their relative force is affecting the constraint
			float velocityDot = Vector3::Dot(relativeAngularVelocity, offsetDir);
			float biasFactor = 0.01f;
			float bias = -(biasFactor / dt) * offset;

			float lambda = -(velocityDot + bias) / constraintMass;
			Vector3 aImpulse = offsetDir * lambda;
			Vector3 bImpulse = -offsetDir * lambda;

			physA->ApplyAngularImpulse(aImpulse);// multiplied by mass here
			physB->ApplyAngularImpulse(bImpulse);// multiplied by mass here

		}

	}

}