#pragma once
#include "..\CSC8503Common\GameObject.h"
#include "GameWorld.h"
#include "PowerUp.h"
#include "BehaviourSequence.h"
#include "BehaviourAction.h"
#include "BehaviourSelector.h"
#include "NavigationGrid.h"
#include "NavigationPath.h"

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

			void MoveEnemy(Vector3 direction, float dt);

			NavigationGrid* getGrid()
			{
				return grid;
			}

		protected:

			vector <Vector3 > testNodes;

			NavigationGrid* grid;
			NavigationPath outPath;
			Vector3 currentWaypoint;

			float speed = 75;

			Vector3 spawn;
			Player* player;
			std::vector<PowerUp*>& powerUps;

			GameObject* target;
			float time;

			BehaviourAction* findTarget;
			BehaviourAction* moveToTarget;
			BehaviourSequence* sequence;
			BehaviourState state;

			class MovementStateAI;

			MovementStateAI* currentState;
			MovementStateAI* normal;
			MovementStateAI* frozenState;
			MovementStateAI* confusedState;

			class MovementStateAI
			{
			public:
				friend class Enemy;
				MovementStateAI(Enemy* self)
				{
					this->self = self;
				}
				virtual void Update(float dt)
				{
					time -= dt;
					if (time < 0)
					{
						Exit();
						self->currentState = self->normal;
					}

				};
				virtual void Move(Vector3 direction, float dt) = 0;
				virtual void Enter() = 0;
				virtual void Exit() = 0;

			protected:
				float time = 5;
				Enemy* self;
			};

			class FrozenAI : public MovementStateAI
			{
			public:
				FrozenAI(Enemy* self) : MovementStateAI(self) {};
				void Move(Vector3 direction, float dt)override;
				void Enter()override;
				void Exit()override;
			};

			class NormalAI : public MovementStateAI
			{
			public:
				NormalAI(Enemy* self) : MovementStateAI(self) {};
				void Move(Vector3 direction, float dt)override;
				void Enter()override;
				void Exit()override;
			};

			class ConfusedAI : public MovementStateAI
			{
			public:
				ConfusedAI(Enemy* self) : MovementStateAI(self) {};
				void Move(Vector3 direction, float dt)override;
				void Enter()override;
				void Exit()override;
			};
		};
	}
}