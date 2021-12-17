#pragma once
#include "..\CSC8503Common\GameObject.h"
namespace NCL {
	namespace CSC8503 {

		class Bouncer : public GameObject {
		public:
			Bouncer(Vector3 axis, Vector3 origin, float cooldown, float force) : cooldown(cooldown)
			{
				this->axis = axis;
				this->force = force;
				timer = -1;
				this->origin = origin;
			}
			std::string OnDebug() override;
			virtual void Update(float dt) override;
			virtual bool OnClick(std::string& information);

		protected:
			float timer;
			const float cooldown;
			Vector3 origin;
			Vector3 axis;
			float force;

		};
	}
}