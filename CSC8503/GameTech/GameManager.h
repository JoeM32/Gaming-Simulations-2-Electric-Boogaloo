#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "StateGameObject.h"
#include "Game.h"
#include "GameA.h"
#include "GameB.h"
#include "TutorialGame.h"
#include "PushdownState.h"
#include "PushdownMachine.h"
#include "../../Common/Window.h"

namespace NCL {
	namespace CSC8503 {
		class  GameManager {
		public:

			GameManager();
			~GameManager();
			bool UpdateGame(float dt);

			void loadGameA();
			void loadGameB();
			void loadTutorialGame();

		private:
			Game* gameData;
			PushdownMachine* menu;
		};


	}
}
