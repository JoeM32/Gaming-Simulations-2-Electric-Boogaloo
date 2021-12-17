#include "PowerUp.h"
#include "..\CSC8503Common\Player.h"
#include "..\CSC8503Common\Enemy.h"

using namespace NCL;
using namespace CSC8503;

void NCL::CSC8503::PowerUp::OnCollisionBegin(GameObject* otherObject)
{
	Player* player = dynamic_cast <Player*> (otherObject);
	Enemy* enemy = dynamic_cast <Enemy*> (otherObject);
	if (player || enemy)
	{
		powerUps.erase(std::remove(powerUps.begin(), powerUps.end(), this), powerUps.end());
		world->RemoveGameObject(this, false);
	}
}
