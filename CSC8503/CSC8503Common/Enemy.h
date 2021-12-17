#pragma once
#include "..\CSC8503Common\GameObject.h"
#include "GameWorld.h"
#include "PowerUp.h"

namespace NCL {
	namespace CSC8503 {

		class Player;

		class Enemy : public GameObject {
		public:
			Enemy(std::vector<PowerUp*>& powerUps);
			~Enemy();

			void SetPlayer(Player* enemy)
			{
				this->player = enemy;

			}
			void SetSpawn(Vector3 spawn)
			{
				this->spawn = spawn;
			}

			virtual void Update(float dt);

			void ApplyEffect(PowerUpType type);

			virtual void OnCollisionBegin(GameObject* otherObject) override;

			virtual std::string OnDebug() override;
		protected:
			Vector3 spawn;
			Player* player;
			std::vector<PowerUp*>& powerUps;
			class MovementState;

			MovementState* currentState;
			MovementState* normal;
			MovementState* frozenState;
			MovementState* confusedState;

			class MovementState
			{
			public:
				friend class Enemy;
				MovementState(Enemy* player)
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
				Enemy* player;
			};

			class Frozen : public MovementState
			{
			public:
				Frozen(Enemy* player) : MovementState(player) {};
				void UpdateEffect(float dt)override;
				void Enter()override;
				void Exit()override;
			};

			class Normal : public MovementState
			{
			public:
				Normal(Enemy* player) : MovementState(player) {};
				void UpdateEffect(float dt)override;
				void Enter()override;
				void Exit()override;
			};

			class Confused : public MovementState
			{
			public:
				Confused(Enemy* player) : MovementState(player) {};
				void UpdateEffect(float dt)override;
				void Enter()override;
				void Exit()override;
			};
		};
	}
}