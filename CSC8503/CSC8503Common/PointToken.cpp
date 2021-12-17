#include "PointToken.h"

using namespace NCL;
using namespace CSC8503;

void NCL::CSC8503::PointToken::OnCollisionBegin(GameObject* otherObject)
{
	PlayerBall* player = dynamic_cast <PlayerBall*> (otherObject);
	if (player)
	{
		counter += 1;

	}
	Player* player1 = dynamic_cast <Player*> (otherObject);//jsut the worst programming ever :/
	if (player1)
	{
		counter += 1;
		
	}
	world->RemoveGameObject(this, false);
}
