#pragma once
#include "Constraint.h"

namespace NCL {
	namespace CSC8503 {
		class GameObject;

		class RotationConstraint : public Constraint {
		public:
			RotationConstraint(GameObject* a, GameObject* b, float d) {
				objectA = a;
				objectB = b;
				rotation = d;
			}
			~RotationConstraint() {}

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* objectA;
			GameObject* objectB;

			float rotation = 0;
		};
	}
}