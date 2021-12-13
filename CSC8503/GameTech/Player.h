#pragma once
#include "..\CSC8503Common\GameObject.h"
namespace NCL {
	namespace CSC8503 {

		class Player : public GameObject {
		public:
			Player();
			~Player();

			virtual void Update(float dt);

		protected:
			class PlayerState;

			PlayerState* state;

			class PlayerState
			{
			public:
				virtual bool Update(float dt)
				{
					time -= dt;
					UpdateEffect(dt);
					if (time < 0)
					{
						return false;
					}
					else
					{
						return true;
					}
				};
				virtual void UpdateEffect(float dt) =0;
				virtual void Enter() = 0;
				virtual void Exit() = 0;
			protected:
				float time = 5;
			};
			class IcePower : PlayerState
			{
			public:
				void UpdateEffect(float dt)override;
				void Enter()override;
				void Exit()override;
			};
			class BouncePower : PlayerState
			{
			public:
				void UpdateEffect(float dt) override;
				void Enter()override;
				void Exit()override;
			};
			class GrapplingHook : PlayerState
			{
			public:
				void UpdateEffect(float dt)override;
				void Enter()override;
				void Exit()override;
			};

			

		};
	}
}