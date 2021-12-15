#include "Bouncer.h"

void NCL::CSC8503::Bouncer::Update(float dt)
{
	timer -= dt;
	if ((transform.GetPosition() - origin).Length() < 0.1)
	{
		transform.SetPosition(origin);
	}
	else
	{
		GetPhysicsObject()->AddForce((transform.GetPosition() - origin).Normalised() * 0.1f);
	}
}

bool NCL::CSC8503::Bouncer::OnClick(std::string& information)
{
	if (timer < 0)
	{
		timer = cooldown;
		GetPhysicsObject()->AddForce((axis).Normalised() * force);
		return true;
	}
	else
	{
		information = "On Cooldown";
		return false;
	}
}
