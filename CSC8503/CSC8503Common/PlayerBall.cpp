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
	this-> GetPhysicsObject()->SetDamping(0.5);

		Ice* ice = dynamic_cast <Ice*> (otherObject);
		if (ice)
		{
			this->GetPhysicsObject()->SetDamping(0.0001);
		}
	
	//std::cout << std::to_string(this->GetPhysicsObject()->GetDamping());
}

std::string NCL::CSC8503::PlayerBall::OnDebug()
{
	return GameObject::OnDebug() + " Player" + " - Damping/Slippiness -" + std::to_string(this->GetPhysicsObject()->GetDamping());
}

bool NCL::CSC8503::PlayerBall::OnClick(std::string& information)
{
	Vector3 direction = Vector3(((rand() % 100) - 50), ((rand() % 100) - 50), ((rand() % 100) - 50)).Normalised();
	this->GetPhysicsObject()->AddForce(direction * 100);
	information = "Nudged ball";
	return true;
}

