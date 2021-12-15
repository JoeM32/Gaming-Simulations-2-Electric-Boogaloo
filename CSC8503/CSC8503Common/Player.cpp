#include "Player.h"
#include "PowerUp.h"

NCL::CSC8503::Player::Player(GameWorld* world)
{
	this->world = world;
	frozenState = new Frozen(this);
	normal = new Normal(this);
	confusedState = new Confused(this);
	grapplestate = new Grapple(this);
	currentState = normal;
}

NCL::CSC8503::Player::~Player()
{
}

void NCL::CSC8503::Player::Update(float dt)
{
	currentState->Update(dt);
}

void NCL::CSC8503::Player::OnCollisionBegin(GameObject* otherObject)
{
	PowerUp* power = dynamic_cast <PowerUp*> (otherObject);
	if (power)
	{
		auto state = normal;
		switch (power->GetType())
		{
		case PowerUpType::Confusion:
			state = confusedState;
			break;
		case PowerUpType::Grapple:
			state = grapplestate;
			break;
		case PowerUpType::Bonus:
			
			break;
		case PowerUpType::Freeze:
			state = frozenState;
			break;
		default:

		}
		currentState->Exit();
		currentState = state;
		currentState->Enter();
	}
}


std::string NCL::CSC8503::Player::OnDebug()
{
	return std::string();
}

void NCL::CSC8503::Player::Frozen::UpdateEffect(float dt)
{
	//do nothing - frozen
}

void NCL::CSC8503::Player::Frozen::Enter()
{
	time = 3;
}

void NCL::CSC8503::Player::Frozen::Exit()
{
}

void NCL::CSC8503::Player::Normal::UpdateEffect(float dt)
{
	time += 2; //yes this is very very dumb, but as is trying to do pathfinifng, FSMs, behaviour trees and automata :'(
	
	Matrix4 view = player->world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Vector3 charForward = player->GetTransform().GetOrientation() * Vector3(0, 0, 1);
	Vector3 charForward2 = player->GetTransform().GetOrientation() * Vector3(0, 0, 1);

	float force = 100.0f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		player->GetPhysicsObject()->AddForce(-rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		player->GetPhysicsObject()->AddForce(rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		player->GetPhysicsObject()->AddForce(fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		player->GetPhysicsObject()->AddForce(-fwdAxis * force);
	}

	

}

void NCL::CSC8503::Player::Normal::Enter()
{
	time = 5;
}

void NCL::CSC8503::Player::Normal::Exit()
{
}

void NCL::CSC8503::Player::Confused::UpdateEffect(float dt)
{
	Matrix4 view = player->world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Vector3 charForward = player->GetTransform().GetOrientation() * Vector3(0, 0, 1);
	Vector3 charForward2 = player->GetTransform().GetOrientation() * Vector3(0, 0, 1);

	float force = 100.0f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		player->GetPhysicsObject()->AddForce(rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		player->GetPhysicsObject()->AddForce(-rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		player->GetPhysicsObject()->AddForce(-fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		player->GetPhysicsObject()->AddForce(fwdAxis * force);
	}
}

void NCL::CSC8503::Player::Confused::Enter()
{
	time = 5;
}

void NCL::CSC8503::Player::Confused::Exit()
{

}

void NCL::CSC8503::Player::Grapple::UpdateEffect(float dt)
{
	time += 2; //yes this is very very dumb, but as is trying to do pathfinifng, FSMs, behaviour trees and automata :'(

	Matrix4 view = player->world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!


	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();



	float force = 100.0f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		//raycast and use temporary spring constrait to pull to wall.
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {

	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {

	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {

	}
}

void NCL::CSC8503::Player::Grapple::Enter()
{
	time = 10;
}

void NCL::CSC8503::Player::Grapple::Exit()
{
}
