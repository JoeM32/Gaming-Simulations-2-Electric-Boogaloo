#include "Enemy.h"
#include "Player.h"
#include "PowerUp.h"
#include "BehaviourSequence.h"
#include "BehaviourAction.h"
#include "BehaviourSelector.h"



NCL::CSC8503::Enemy::Enemy(std::vector<PowerUp*>& powerUps) : powerUps(powerUps)
{
	float behaviourTimer;
	float distanceToTarget;
	BehaviourAction* findTarget = new BehaviourAction("Find Target",
		[&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				std::cout << "SelectingTarget\n";
				return Success;
				
			}
			return state; 
		}
	);
	BehaviourAction* moveToTarget = new BehaviourAction("move To Target",
		[&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				std::cout << "Heading to target!\n";
				state = Ongoing;

			}
			else if (state == Ongoing) {
				distanceToTarget -= dt;
				if (distanceToTarget <= 0.0f) {
					std::cout << "Reached room!\n";
					return Success;

				}

			}
			return state; //will be ’ongoing ’ until success
		}
	);
	BehaviourSequence* sequence =
		new BehaviourSequence("Room Sequence");
	sequence->AddChild(findKey);
	sequence->AddChild(goToRoom);
	sequence->AddChild(openDoor);

	BehaviourSelector* selection =
		new BehaviourSelector("Loot Selection");
	selection->AddChild(lookForTreasure);
	selection->AddChild(lookForItems);

	BehaviourSequence* rootSequence =
		new BehaviourSequence("Root Sequence");
	rootSequence->AddChild(sequence);
	rootSequence->AddChild(selection);
	for (int i = 0; i < 5; ++i) {
		rootSequence->Reset();
		behaviourTimer = 0.0f;
		distanceToTarget = rand() % 250;
		BehaviourState state = Ongoing;
		std::cout << "We’re going on an adventure !\n";
		while (state == Ongoing) {
			state = rootSequence->Execute(1.0f); //fake dt

		}
		if (state == Success) {
			std::cout << "What a successful adventure !\n";

		}
		else if (state == Failure) {
			std::cout << "What a waste of time!\n";

		}
	}
	std::cout << "All done!\n";
}

NCL::CSC8503::Enemy::~Enemy()
{
}

void NCL::CSC8503::Enemy::Update(float dt)
{
}

void NCL::CSC8503::Enemy::ApplyEffect(PowerUpType type)
{
}

void NCL::CSC8503::Enemy::OnCollisionBegin(GameObject* otherObject)
{
	PowerUp* power = dynamic_cast <PowerUp*> (otherObject);
	if (power)
	{
		std::cout << "PowerUp";
	}
	if (otherObject == player)
	{
		std::cout << "Player";
		this->transform.SetPosition(spawn);
	}

}

std::string NCL::CSC8503::Enemy::OnDebug()
{
	return std::string();
}
