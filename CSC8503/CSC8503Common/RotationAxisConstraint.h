#pragma once
#include "Transform.h"
#include "GameObject.h"
#include "../../Common/Maths.h"
#include "Constraint.h"

namespace NCL {
	namespace CSC8503 {
		class GameObject;

		class RotationAxisConstraint : public Constraint {
		public:
			RotationAxisConstraint(GameObject* object, Vector3 axis) {
				this->object = object;
				this->axis = axis;

			}
			~RotationAxisConstraint() {}

			bool UpdateConstraint(float dt) override;

		protected:
			GameObject* object;
			Vector3 axis;
		};
	}
}