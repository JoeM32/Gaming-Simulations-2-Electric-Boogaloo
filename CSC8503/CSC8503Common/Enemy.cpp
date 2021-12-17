#include "Enemy.h"
#include "Player.h"
#include "PowerUp.h"
#include "Debug.cpp"


NCL::CSC8503::Enemy::Enemy(std::vector<PowerUp*>& powerUps) : powerUps(powerUps)
{
	grid = new NavigationGrid("Grid1.txt");
	//Vector3 offset = Vector3(-100, 2, -100);

	/*Vector3 startPos(10, 0, 10);
	Vector3 endPos(80, 0, 80);
	startPos *= 2;
	endPos *= 2;
	startPos += offset;
	endPos += offset;*/
	//grid->SetOffset(offset);

	findTarget = new BehaviourAction("Find Target",
		[&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				//std::cout << "SelectingTarget\n" << powerUps.size();
				float distanceToPlayer = (transform.GetPosition() - player->GetTransform().GetPosition()).Length();//ignore walls so is actaully super dumb. like me
				//std::cout << std::to_string(distanceToPlayer);
				if (distanceToPlayer < 60)
				{
					target = player;
				}
				else
				{
					float closestPowerup = distanceToPlayer + 1;
					for (auto it = powerUps.begin(); it != powerUps.end(); ++it) 
					{
						float dist = ((**it).GetTransform().GetPosition() - transform.GetPosition()).Length();
						if (dist < closestPowerup)
						{
							closestPowerup = dist;
							target = (*it);
						}
					}
					if (closestPowerup > distanceToPlayer)
					{
						target = player;
					}
				}
				if (!target)
				{
					target = player;
				}
				//std::cout << "Found target!" << target->GetName() << "\n";
				return Success;

			}
			return state;
		}
	);
	moveToTarget = new BehaviourAction("move To Target",
		[&](float dt, BehaviourState state)->BehaviourState {

			if (state == Initialise) {
				time = 0;

				outPath.Clear();
	

				//Inefficent but useful to see AI decisions
				Vector3 pos;
				grid->FindPath(transform.GetPosition(), target->GetTransform().GetPosition(), outPath);
				testNodes.clear();
				while (outPath.PopWaypoint(pos)) {
					testNodes.push_back(pos);
					std::cout << "\n" << pos;
				}



				bool found = grid->FindPath(transform.GetPosition(), target->GetTransform().GetPosition(), outPath); 
				if (found)
				{
					outPath.PopWaypoint(currentWaypoint);
					state = Ongoing;
				}
				else
				{
					state = Failure;
				}

			}
			else if (state == Ongoing) {
				time += dt;
				if (time < 1)
				{
					//std::cout << "executing path!\n";
					for (int i = 1; i < testNodes.size(); ++i) {
						Vector3 a = testNodes[i - 1];
						Vector3 b = testNodes[i];

						Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
					}

					if ((currentWaypoint - transform.GetPosition()).Length() < 10)
					{
						if (!outPath.PopWaypoint(currentWaypoint))
						{
							return Success; //made it
						}
						return Ongoing;
					}
					else
					{
						//move to it
						this->MoveEnemy((currentWaypoint - transform.GetPosition()).Normalised(), dt);
						return Ongoing;
					}
				}
				else
				{
					return Failure;
				}
				
				
			}
			return state; //will be ’ongoing ’ until success
		}
	);
	sequence =
		new BehaviourSequence("Main sequence");
	sequence->AddChild(findTarget);
	sequence->AddChild(moveToTarget);
	sequence->Reset();
	state = Ongoing;



	frozenState = new FrozenAI(this);
	normal = new NormalAI(this);
	confusedState = new ConfusedAI(this);
	currentState = normal;
}

NCL::CSC8503::Enemy::~Enemy()
{
}

void NCL::CSC8503::Enemy::Update(float dt)
{
	state = sequence->Execute(dt); //fake dt
	if (state != Ongoing) //whether we got to where we are going or not we must decide again
	{
		sequence->Reset();
		state = Ongoing;
	}
	currentState->Update(dt);

}

void NCL::CSC8503::Enemy::ApplyEffect(PowerUpType type)
{
	std::cout << "Enemy Affected";
	auto state = normal;
	switch (type)
	{
	case PowerUpType::Confusion:
		state = confusedState;
		break;
	case PowerUpType::Freeze:
		state = frozenState;
		break;
	default:
		break;
	};

	currentState->Exit();
	currentState = state;
	currentState->Enter();
}

void NCL::CSC8503::Enemy::OnCollisionBegin(GameObject* otherObject)
{
	PowerUp* power = dynamic_cast <PowerUp*> (otherObject);
	if (power)
	{
		std::cout << "PowerUp";
		player->ApplyEffect(power->GetType());
	}
	if (otherObject == player)
	{
		std::cout << "Player hit returing to" << spawn << "\n";
		this->transform.SetPosition(spawn);
	}

}

std::string NCL::CSC8503::Enemy::OnDebug()
{
	return std::string();
}

void NCL::CSC8503::Enemy::MoveEnemy(Vector3 direction, float dt)
{
	currentState->Move(direction, dt);
}

void NCL::CSC8503::Enemy::FrozenAI::Move(Vector3 direction, float dt)
{
	//do nothing - frozen
}

void NCL::CSC8503::Enemy::FrozenAI::Enter()
{
	time = 3;
}

void NCL::CSC8503::Enemy::FrozenAI::Exit()
{
}

void NCL::CSC8503::Enemy::NormalAI::Move(Vector3 direction, float dt)
{
	self->GetPhysicsObject()->AddForce(direction.Normalised() * self->speed);
}

void NCL::CSC8503::Enemy::NormalAI::Enter()
{
	time = 5;
}

void NCL::CSC8503::Enemy::NormalAI::Exit()
{
}

void NCL::CSC8503::Enemy::ConfusedAI::Move(Vector3 direction, float dt)
{
	self->GetPhysicsObject()->AddForce((direction * Vector3(-1,0,-1)).Normalised() * self->speed);
}

void NCL::CSC8503::Enemy::ConfusedAI::Enter()
{
	time = 5;
}

void NCL::CSC8503::Enemy::ConfusedAI::Exit()
{

}