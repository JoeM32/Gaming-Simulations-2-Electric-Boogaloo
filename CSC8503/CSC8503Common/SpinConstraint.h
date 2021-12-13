#pragma once
#include "Transform.h"
#include "GameObject.h"
#include "../../Common/Maths.h"
#include "Constraint.h"

namespace NCL {
	namespace CSC8503 {
		class GameObject;

		class SpinConstraint : public Constraint {
		public:
			SpinConstraint(GameObject* object, Vector3 axis, float speed) {
				this->object = object;
				this->axis = axis;
				this->speed;
			}
			~SpinConstraint() {}

			bool UpdateConstraint(float dt) override;

		protected:
			GameObject* object;
			Vector3 axis;
			float speed;
		};
	}
}