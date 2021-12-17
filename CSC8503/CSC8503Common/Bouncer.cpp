#include "Bouncer.h"

void NCL::CSC8503::Bouncer::Update(float dt)
{
	timer -= dt;
	if (cooldown - timer < cooldown/50)
	{
		//std::cout << "pushing";
		GetPhysicsObject()->AddForce((axis).Normalised() * force);
	}
	else
	{
	
		if ((transform.GetPosition() - origin).Length() > 0.1f)
		{
			//std::cout << "resetting";
			GetPhysicsObject()->AddForce((origin - transform.GetPosition()).Normalised() * force/20);
		}
		else
		{
			//std::cout << "home";
			transform.SetPosition(origin);
		}
	}
}

bool NCL::CSC8503::Bouncer::OnClick(std::string& information)
{

	if (timer < 0)
	{
		timer = cooldown;
		GetPhysicsObject()->AddForce((axis).Normalised() * force);
		information = "firing";
		return true;
	}
	else
	{
		information = "On Cooldown";
		return false;
	}
}

std::string NCL::CSC8503::Bouncer::OnDebug()
{
	return GameObject::OnDebug() + " Bouncer" + " - Cooldown " + std::to_string(timer > 0 ? timer : 0);
}
