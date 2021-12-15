#pragma once
#include "..\CSC8503Common\GameObject.h"
#include "..\CSC8503Common\Player.h"
#include "../../Common/Window.h"
#include "../GameTech/Game.h"


namespace NCL {
	namespace CSC8503 {

		enum class PowerUpType {
			Confusion = 1,
			Grapple = 2,
			Bonus = 4,
			Freeze = 8,
		};

		class PowerUp : public GameObject {
		public:
			PowerUp(PowerUpType type, Game* game)
			{
				this->type = type;
				this->game = game;//this is now hilariously bad, making the game depend on powerups lmao. needs must when the devil drives - i.e. doing 20 credit module 100% coursework in a week and a half. This will never, ever, ever go on my portfolio thank god
			}

			void OnCollisionBegin(GameObject* otherObject) override;

			PowerUpType GetType()
			{
				return type;
			}
		protected:
			PowerUpType type;
			Game* game;
		};
	}
}
