#include "GameManager.h"
using namespace NCL;
using namespace CSC8503;


class InGameA : public PushdownState {
public:
	InGameA(Game* gameData) : PushdownState(gameData) {};

	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		game->UpdateGame(dt);
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)) {
			std::cout << "Esc";
			return PushdownResult::Pop;
		}
		return PushdownResult::NoChange;
	}
	void OnAwake() override {
		game = new GameA(gameData->GetGameWorld(), gameData->GetRenderer(), gameData->GetPhysics());
		std::cout << "Game Loaded";
	}

	void OnSleep() override {
		delete game;
		gameData->GetGameWorld()->ClearAndErase();

	}
protected:
	Game* game;

};

class InGameB : public PushdownState {
public:
	InGameB(Game* gameData) : PushdownState(gameData) {};

	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		game->UpdateGame(dt);
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)) {
			std::cout << "Esc";
			return PushdownResult::Pop;
		}
		return PushdownResult::NoChange;
	}
	void OnAwake() override {
		game = new GameB(gameData->GetGameWorld(), gameData->GetRenderer(), gameData->GetPhysics());
		std::cout << "Game Loaded";
	}

	void OnSleep() override {
		delete game;
		gameData->GetGameWorld()->ClearAndErase();

	}
protected:
	Game* game;

};


class LevelSelect : public PushdownState {
public:
	LevelSelect(Game* gameData) : PushdownState(gameData) {};

	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {

		

		gameData->GetRenderer()->DrawString("Level Select", Vector2(10, 10));
		gameData->GetRenderer()->DrawString("Level A", Vector2(10, 30), !selected ? Vector4(1,1,1,1) : Vector4(0, 1, 0, 1));
		gameData->GetRenderer()->DrawString("Level B", Vector2(10, 40), selected ? Vector4(1, 1, 1, 1) : Vector4(0, 1, 0, 1));
		gameData->GetRenderer()->DrawString("Press Enter to play", Vector2(10, 60));
		gameData->GetRenderer()->DrawString("Press Escape to exit", Vector2(10, 70));

		gameData->UpdateGame(dt);

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::DOWN) || Window::GetKeyboard()->KeyPressed(KeyboardKeys::UP)) {
			selected = !selected;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::RETURN)) {
			if (selected)
			{
				std::cout << "GameA";
				*newState = new InGameA(gameData);
			}
			else
			{
				std::cout << "GameB";
				*newState = new InGameB(gameData);
			}
			return PushdownResult::Push;

		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)) {
			std::cout << "Esc";
			return PushdownResult::Pop;
		}
		return PushdownResult::NoChange;

	};
	void OnAwake() override {


	}
protected:
	bool selected = true;
};

class MainScreen : public PushdownState {
public:
	MainScreen(Game* gameData) : PushdownState(gameData) {};

	PushdownResult OnUpdate(float dt, PushdownState** newState) override
	{
		gameData->GetRenderer()->DrawString("Main Menu", Vector2(10, 10));
		gameData->GetRenderer()->DrawString("Press enter for level Select", Vector2(10, 20));
		gameData->GetRenderer()->DrawString("Press esc to exit", Vector2(10, 30));

		gameData->UpdateGame(dt);

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::RETURN)) {
			*newState = new LevelSelect(gameData);
			return PushdownResult::Push;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)) {
			return PushdownResult::Pop;

		}
		
		return PushdownResult::NoChange;

	};

	void OnAwake() override {


	}


};



NCL::CSC8503::GameManager::GameManager()
{
	GameWorld* world = new GameWorld();
	GameTechRenderer* renderer = new GameTechRenderer(*world);
	PhysicsSystem* physics = new PhysicsSystem(*world);
	Debug::SetRenderer(renderer);
	gameData = new Game(world, renderer, physics);
	menu = new PushdownMachine((new MainScreen(gameData)));
}

NCL::CSC8503::GameManager::~GameManager()
{
	delete gameData;
}

bool NCL::CSC8503::GameManager::UpdateGame(float dt)
{
	//std::cout << "updatingLevel";
	return !menu->Update(dt);
}
