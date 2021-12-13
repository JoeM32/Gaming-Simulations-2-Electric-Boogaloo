#pragma once
#include "Transform.h"
#include "GameObject.h"
#include "../../Common/Maths.h"
#include "Constraint.h"

namespace NCL {
	namespace CSC8503 {
		class GameObject;

		class PositionAxisConstraint : public Constraint {
		public:
			PositionAxisConstraint(GameObject* object, Vector3 axis) {
				this->object = object;
				this->axis = axis;

			}
			~PositionAxisConstraint() {}

			bool UpdateConstraint(float dt) override;

		protected:
			GameObject* object;
			Vector3 axis;
		};
	}
}