#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "Game.h"

namespace NCL {
	namespace CSC8503 {
		class  Game {
		public:
			Game(GameWorld* gameworld, GameTechRenderer* renderer, PhysicsSystem* physics)
			{
				this->gameworld = gameworld;
				this->renderer = renderer;
				this->physics = physics;
			};

			void Reset()
			{
				gameworld = new GameWorld();
				renderer = new GameTechRenderer(*gameworld);
				physics = new PhysicsSystem(*gameworld);
			}

			GameTechRenderer* GetRenderer()
			{
				return renderer;
			}

			GameWorld* GetGameWorld()
			{
				return gameworld;
			}


			PhysicsSystem* GetPhysics()
			{
				return physics;
			}


			virtual void UpdateGame(float dt) {
				renderer->Update(dt);
				Debug::FlushRenderables(dt);
				renderer->Render();
			};

			virtual void BonusAccquired() {};

		private://yes this is double references. This class was not built to work like this, but the menu has forced my hand at the eleventh hour.
			GameWorld* gameworld;
			GameTechRenderer* renderer;
			PhysicsSystem* physics;
		};
	}
}