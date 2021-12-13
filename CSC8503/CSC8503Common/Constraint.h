#pragma once

namespace NCL {
	namespace CSC8503 {
		class Constraint	{
		public:
			Constraint() {}
			virtual ~Constraint() {}

			virtual bool UpdateConstraint(float dt) = 0;
		};
	}
}