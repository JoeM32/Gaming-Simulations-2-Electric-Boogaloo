#include "PowerUp.h"

void NCL::CSC8503::PowerUp::OnCollisionBegin(GameObject* otherObject)
{
	Player* player = dynamic_cast <Player*> (otherObject);
	if (player)
	{
		if (type == PowerUpType::Bonus)
		{
			game->BonusAccquired();
		}
		game->GetGameWorld()->RemoveGameObject(this, true);
	}
}
