#pragma once
#include "..\CSC8503Common\GameObject.h"
namespace NCL {
	namespace CSC8503 {

		class PlayerBall : public GameObject {
		public:
			PlayerBall();
			~PlayerBall();

			virtual void Update(float dt);

			virtual void OnCollisionStay(GameObject* otherObject) override;

			virtual std::string OnDebug() override;

			virtual bool OnClick(std::string& information);
		protected:

		};
	}
}