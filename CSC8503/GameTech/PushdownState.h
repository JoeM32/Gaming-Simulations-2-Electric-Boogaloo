#pragma once
#include "Game.h"

namespace NCL {
	namespace CSC8503 {
		class PushdownState {
		public:
			enum PushdownResult {
				Push, Pop, NoChange

			};
			PushdownState(NCL::CSC8503::Game* gameData) {
				this->gameData = gameData;
			}
			virtual ~PushdownState() {};

			virtual PushdownResult OnUpdate(float dt,
				PushdownState** pushFunc) = 0;
			virtual void OnAwake() {};
			virtual void OnSleep() {};

		protected:
			NCL::CSC8503::Game* gameData;
		};
	}
}

