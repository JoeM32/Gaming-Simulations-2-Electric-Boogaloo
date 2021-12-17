#pragma once
#include "..\CSC8503Common\GameObject.h"
#include "GameWorld.h"

namespace NCL {
	namespace CSC8503 {

		class Player;
		class Enemy;


		class PowerUp : public GameObject {
		public:
			PowerUp(PowerUpType type, GameWorld* game, std::vector<PowerUp*>& powerUps) : powerUps(powerUps)
			{
				this->type = type;
				this->world = game;//this is now hilariously bad, making the game depend on powerups lmao. needs must when the devil drives - i.e. doing 20 credit module 100% coursework in a week and a half. This will never, ever, ever go on my portfolio thank god
			}

			void OnCollisionBegin(NCL::CSC8503::GameObject* otherObject) override;

			PowerUpType GetType()
			{
				return type;
			}
		protected:
			PowerUpType type;
			GameWorld* world;
			std::vector<PowerUp*>& powerUps;
		};
	}
}
