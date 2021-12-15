#pragma once
#include "..\CSC8503Common\GameObject.h"
#include "../../Common/Window.h"
#include "GameWorld.h"

namespace NCL {
	namespace CSC8503 {

		class Player : public GameObject {
		public:
			Player(GameWorld* world);
			~Player();

			virtual void Update(float dt);

			virtual void OnCollisionBegin(GameObject* otherObject) override;

			virtual std::string OnDebug() override;
		protected:

			GameWorld* world;

			class MovementState;

			MovementState* currentState;

			MovementState* normal;
			MovementState* frozenState;
			MovementState* confusedState;
			MovementState* grapplestate;

			class MovementState
			{
			public:
				friend class Player;
				MovementState(Player* player)
				{
					this->player = player;
				}
				virtual void Update(float dt)
				{
					time -= dt;
					UpdateEffect(dt);
					if (time < 0)
					{
						Exit();
						player->currentState = player->normal;
					}
					
				};
				virtual void UpdateEffect(float dt) = 0;
				virtual void Enter() = 0;
				virtual void Exit() = 0;

			protected:
				float time = 5;
				Player* player;
			};

			class Frozen : public MovementState
			{
			public:
				Frozen(Player* player) : MovementState(player){};
				void UpdateEffect(float dt)override;
				void Enter()override;
				void Exit()override;
			};
			
			class Normal : public MovementState
			{
			public:
				Normal(Player* player) : MovementState(player) {};
				void UpdateEffect(float dt)override;
				void Enter()override;
				void Exit()override;
			};

			class Confused : public MovementState
			{
			public:
				Confused(Player* player) : MovementState(player) {};
				void UpdateEffect(float dt)override;
				void Enter()override;
				void Exit()override;
			};

			class Grapple : public MovementState
			{
			public:
				Grapple(Player* player) : MovementState(player) {};
				void UpdateEffect(float dt)override;
				void Enter()override;
				void Exit()override;
			};



		};
	}
}