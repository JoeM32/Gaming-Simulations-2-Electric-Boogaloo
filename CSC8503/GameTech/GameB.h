#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/PowerUp.h"
#include "../CSC8503Common/Player.h"
#include "../CSC8503Common/Enemy.h"
#include "StateGameObject.h"
#include "Game.h"

namespace NCL {
	namespace CSC8503 {
		class GameB : public Game {
		public:
			GameB(GameWorld* gameworld, GameTechRenderer* renderer, PhysicsSystem* physics);
			~GameB();

			virtual void UpdateGame(float dt) override;

		protected:
			int score;
			float timer = 0;
			void CameraLook();
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			void InitWorld();

			void InitExamples();

			void CreateMap();


			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitDefaultFloor();
			void BridgeConstraintTest();

			bool SelectObject();
			bool DebugObjects();
			bool ClickObjects();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			void DisplayGrid();
			void BuildWalls();

			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);
			GameObject* AddOBBToWorld(const Vector3& position, Vector3 rotation, Vector3 dimensions, float inverseMass = 10.0f);

			GameObject* AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f);

			GameObject* AddPlayerToWorld();
			GameObject* AddEnemyToWorld();
			GameObject* AddPointToWorld(const Vector3& position);
			GameObject* AddPowerUpToWorld(const Vector3& position);
			//GameObject* AddBonusToWorld(const Vector3& position, PowerUpType type);

			GameTechRenderer* renderer;
			PhysicsSystem* physics;
			GameWorld* world;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			std::string debugText = "Right Click to Debug";

			OGLMesh* capsuleMesh = nullptr;
			OGLMesh* cubeMesh = nullptr;
			OGLMesh* sphereMesh = nullptr;
			OGLTexture* basicTex = nullptr;
			OGLShader* basicShader = nullptr;

			//Coursework Meshes
			OGLMesh* charMeshA = nullptr;
			OGLMesh* charMeshB = nullptr;
			OGLMesh* enemyMesh = nullptr;
			OGLMesh* bonusMesh = nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject = nullptr;
			Vector3 lockedOffset = Vector3(0, 14*5, 20*5);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

			StateGameObject* AddStateObjectToWorld(const Vector3& position);
			StateGameObject* testStateObject;

			Player* player;
			Enemy* enemy;

			std::vector<PowerUp*> powerUps;

			Vector3 playerSpawn;
			Vector3 enemySpawn;

			int Score;
		};
	}
}

