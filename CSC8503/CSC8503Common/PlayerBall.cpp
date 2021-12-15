#include "PlayerBall.h"
#include "Ice.h"
#include "../../Common/Mouse.h"

NCL::CSC8503::PlayerBall::PlayerBall()
{
}

NCL::CSC8503::PlayerBall::~PlayerBall()
{
}

void NCL::CSC8503::PlayerBall::Update(float dt)
{
}

void NCL::CSC8503::PlayerBall::OnCollisionStay(GameObject* otherObject)
{
	this-> GetPhysicsObject()->SetDamping(0.4);
	Ice* ice = dynamic_cast <Ice*> (otherObject);
	if (ice)
	{
		this->GetPhysicsObject()->SetDamping(0.0001);
	}
}

std::string NCL::CSC8503::PlayerBall::OnDebug()
{
	return GameObject::OnDebug() + " Player";
}

