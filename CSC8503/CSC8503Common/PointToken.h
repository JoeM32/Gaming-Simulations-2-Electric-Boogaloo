#pragma once
#include "..\CSC8503Common\GameObject.h"
#include "..\CSC8503Common\PlayerBall.h"
#include "..\CSC8503Common\Player.h"

namespace NCL {
	namespace CSC8503 {

		class PointToken : public GameObject {
		public:
			PointToken(GameWorld* game, int& counter) : counter(counter)
			{
				this->world = game;//this is now hilariously bad, making the game depend on powerups lmao. needs must when the devil drives - i.e. doing 20 credit module 100% coursework in a week and a half. This will never, ever, ever go on my portfolio thank god
			}

			void OnCollisionBegin(NCL::CSC8503::GameObject* otherObject) override;


		protected:
			int& counter;
			GameWorld* world;
		};
	}
}

