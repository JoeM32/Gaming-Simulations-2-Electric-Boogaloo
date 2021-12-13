#pragma once
#include "Transform.h"
#include "GameObject.h"
#include "../../Common/Maths.h"
#include "Constraint.h"

namespace NCL {
	namespace CSC8503 {
		class GameObject;

		class SpringConstraint : public Constraint {
		public:
			SpringConstraint(GameObject* object1, GameObject* object2, float length = 0.0f, float snappiness = 0.5f, float damping = 0.5f) {
				this->object1 = object1;
				this->object2 = object2;
				restingLength = length;
				this->snappiness = snappiness;
				this->damping = damping;
			}
			~SpringConstraint() {}

			bool UpdateConstraint(float dt) override;

			void SetLength(float length) {
				restingLength = length;
			}
			void SetSnappiness(float snappiness)
			{
				this->snappiness = snappiness;
			}
			void SetDamping(float damping)
			{
				this->damping = damping;
			}

		protected:
			GameObject* object1;
			GameObject* object2;
			float restingLength = 0;
			float snappiness = 0.5f;
			float damping = 0.5f;
		};
	}
}