#pragma once
#include "Transform.h"
#include "GameObject.h"
#include "../../Common/Maths.h"
#include "Constraint.h"

namespace NCL {
	namespace CSC8503 {
		class GameObject;

		class HookConstraint : public Constraint {
		public:
			HookConstraint(GameObject* object1, Vector3 point, float time = 1.0f, float length = 0.0f, float snappiness = 0.5f, float damping = 0.5f) {
				 
				this->point = point;
				object = object1;
				timer = time;
				restingLength = length;
				this->snappiness = snappiness;
				this->damping = damping;
			}
			~HookConstraint() {}

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
			GameObject* object;
			Vector3 point;
			float timer;
			float restingLength = 0;
			float snappiness = 0.5f;
			float damping = 0.5f;
		};
	}
}