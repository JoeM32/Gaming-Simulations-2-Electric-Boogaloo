#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "StateGameObject.h"
#include "Game.h"

namespace NCL {
	namespace CSC8503 {
		class GameA : public Game {
		public:
			GameA(GameWorld* gameworld, GameTechRenderer* renderer, PhysicsSystem* physics);
			~GameA();

			virtual void UpdateGame(float dt) override;

		protected:
			void InitialiseAssets();
			void InitCamera();
			void UpdateKeys();

			void InitWorld();

			void InitOne();

			void CreateMap();

			void InitGameExamples();

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
			 



			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f, bool clipping = false);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, bool clipping = false);
			GameObject* AddIceToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 0.0f);
			GameObject* AddBouncerToWorld(const Vector3& position, Vector3 axis, float size);
			GameObject* AddOBBToWorld(const Vector3& position, Vector3 rotation, Vector3 dimensions, float inverseMass = 10.0f);

			GameObject* AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f);

			GameObject* AddPlayerToWorld(const Vector3& position);
			GameObject* AddEnemyToWorld(const Vector3& position);
			GameObject* AddPointToWorld(const Vector3& position);

			GameTechRenderer* renderer;
			PhysicsSystem* physics;
			GameWorld* world;

			GameObject* player;
			Vector3 spawn;

			bool useGravity;
			bool inSelectionMode;

			int score;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			std::string debugText = "Right Click to Debug";
			std::string clickText = "";

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
			Vector3 lockedOffset = Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

			StateGameObject* AddStateObjectToWorld(const Vector3& position);
			StateGameObject* testStateObject;


			float timer;

		};
	}
}

