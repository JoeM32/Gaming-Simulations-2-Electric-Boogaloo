#include "Player.h"

NCL::CSC8503::Player::Player()
{
	frozenState = new Frozen(this);
}
