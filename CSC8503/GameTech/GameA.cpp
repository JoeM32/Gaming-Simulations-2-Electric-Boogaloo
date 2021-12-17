#include "GameA.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "../CSC8503Common/RotationConstraint.h"
#include "../CSC8503Common/PositionAxisConstraint.h"
#include "../CSC8503Common/RotationAxisConstraint.h"
#include "../CSC8503Common/VelocityAxisConstraint.h"
#include "../CSC8503Common/PlayerBall.h"
#include "../CSC8503Common/PointToken.h"
#include "../CSC8503Common/Ice.h"
#include "../CSC8503Common/Bouncer.h"

using namespace NCL;
using namespace CSC8503;

GameA::GameA(GameWorld* gameworld, GameTechRenderer* renderer, PhysicsSystem* physics) : Game(gameworld, renderer, physics) {
	world = gameworld;
	this->renderer = renderer;
	this->physics = physics;

	forceMagnitude = 10.0f;
	useGravity = true;
	inSelectionMode = false;

	timer = 0;
	score = 0;

	spawn = Vector3(0, 0, 0);

	//Debug::SetRenderer(renderer);
	InitialiseAssets();


}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes,
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void GameA::InitialiseAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh", &cubeMesh);
	loadFunc("sphere.msh", &sphereMesh);
	loadFunc("Male1.msh", &charMeshA);
	loadFunc("courier.msh", &charMeshB);
	loadFunc("security.msh", &enemyMesh);
	loadFunc("coin.msh", &bonusMesh);
	loadFunc("capsule.msh", &capsuleMesh);

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	InitCamera();
	InitWorld();
}



GameA::~GameA() {
	delete cubeMesh;
	delete sphereMesh;
	delete charMeshA;
	delete charMeshB;
	delete enemyMesh;
	delete bonusMesh;

	delete basicTex;
	delete basicShader;

	
}

void GameA::UpdateGame(float dt) {


	if (score < 3)
	{
		world->GetMainCamera()->UpdateCamera(dt);
		timer += dt;
		DebugObjects();
		ClickObjects();
		UpdateKeys();
	}
	else
	{
		renderer->DrawString("You won in " + std::to_string(timer), Vector2(30, 30));
		renderer->DrawString("Esc to return to level select", Vector2(30, 50));
	}

	

	/*if (useGravity) {
		Debug::Print("(G)ravity on", Vector2(5, 95));
	}
	else {
		Debug::Print("(G)ravity off", Vector2(5, 95));
	}*/

	//SelectObject();
	//MoveSelectedObject();

	
	physics->Update(dt);

	renderer->DrawString(debugText, Vector2(2, 70),Vector4(1,0.2,0.2,1),10.0f);
	renderer->DrawString(clickText, Vector2(2, 90));

	/*if (lockedObject != nullptr) {
		Vector3 objPos = lockedObject->GetTransform().GetPosition();
		Vector3 camPos = objPos + lockedOffset;

		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0, 1, 0));

		Matrix4 modelMat = temp.Inverse();

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!

		world->GetMainCamera()->SetPosition(camPos);
		world->GetMainCamera()->SetPitch(angles.x);
		world->GetMainCamera()->SetYaw(angles.y);

		//Debug::DrawAxisLines(lockedObject->GetTransform().GetMatrix(), 2.0f);
	}*/

	world->UpdateWorld(dt);
	renderer->Update(dt);

	Debug::FlushRenderables(dt);
	renderer->Render();



	/*if (testStateObject) {
		testStateObject->Update(dt);
	}*/
}

void GameA::UpdateKeys() {
	/*if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
		selectionObject = nullptr;
		lockedObject = nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
		useGravity = !useGravity; //Toggle gravity!
		physics->UseGravity(useGravity);
	}
	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}

	if (lockedObject) {
		LockedObjectMovement();
	}
	else {
		DebugObjectMovement();
	}*/
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::R)) {
		player->GetTransform().SetPosition(spawn);
		timer += 5;
	}


}

void GameA::LockedObjectMovement() {
	Matrix4 view = world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Vector3 charForward = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);
	Vector3 charForward2 = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);

	float force = 100.0f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		lockedObject->GetPhysicsObject()->AddForce(-rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		Vector3 worldPos = selectionObject->GetTransform().GetPosition();
		lockedObject->GetPhysicsObject()->AddForce(rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		lockedObject->GetPhysicsObject()->AddForce(fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		lockedObject->GetPhysicsObject()->AddForce(-fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NEXT)) {
		lockedObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
	}
}



void GameA::DebugObjectMovement() {
	//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject) {
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM7)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM8)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM5)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}

}

void GameA::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));
	//world->GetMainCamera()->SetPosition(Vector3(500, 500, 500));
	lockedObject = nullptr;
}

void GameA::InitWorld() {

	Window::GetWindow()->ShowOSPointer(true);
	physics->UseGravity(true);
	world->ClearAndErase();
	physics->Clear();

	//InitMixedGridWorld(5, 5, 3.5f, 3.5f);
	//InitGameExamples();
	//InitDefaultFloor();
	//InitOne
	CreateMap();
	AddPlayerToWorld(spawn);
	AddPointToWorld(Vector3(30,2,30));
	AddPointToWorld(Vector3(-30, 2, -30));
	AddPointToWorld(Vector3(30, 2, -30));
	//BridgeConstraintTest();

	//testStateObject = AddStateObjectToWorld(Vector3(0, 10, 0));

}

void NCL::CSC8503::GameA::InitOne()
{
	float sphereRadius = 1.0f;

	//GameObject* sphere = AddSphereToWorld(Vector3(0,5,0), sphereRadius);
	GameObject* OBB1 = AddOBBToWorld(Vector3(0, 0.5, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));

	GameObject* OBB2 = AddOBBToWorld(Vector3(0, 5, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));

	//GameObject* OBB2 = AddSphereToWorld(Vector3(0, 5, 0), 1.0f);
}

void NCL::CSC8503::GameA::CreateMap()
{
	GameObject* obj;
	AddCubeToWorld(Vector3(0, 0, 100), Vector3(100, 20, 5),0, true);
	AddCubeToWorld(Vector3(0, 0, -100), Vector3(100, 20, 5), 0, true);
	AddCubeToWorld(Vector3(100, 0, 0), Vector3(5, 20, 100), 0, true);
	AddCubeToWorld(Vector3(-100, 0, 0), Vector3(5, 20, 100), 0, true);
	obj = AddIceToWorld(Vector3(0, -5, -25), Vector3(100, 2, 25), 0);
	obj->GetRenderObject()->SetColour(Vector4(0.3, 0.3, 1, 1));
	AddCubeToWorld(Vector3(0, -5, -75), Vector3(100, 2, 25), 0, true);
	AddCubeToWorld(Vector3(0, -5, 25), Vector3(100, 2, 25), 0, true);
	obj = AddIceToWorld(Vector3(0, -5, 75), Vector3(100, 2, 25), 0);
	obj->GetRenderObject()->SetColour(Vector4(0.3, 0.3, 1, 1));

	/*obj = AddSphereToWorld(Vector3(0, -5, 100), 30, 0);
	obj->GetPhysicsObject()->SetElasticity(1.0f);
	obj->GetRenderObject()->SetColour(Vector4(0.5, 0.3, 0.2, 1));
	obj = AddSphereToWorld(Vector3(0, -5, -100), 30, 0);
	obj->GetPhysicsObject()->SetElasticity(1.0f);
	obj->GetRenderObject()->SetColour(Vector4(0.5, 0.3, 0.2, 1));
	obj = AddSphereToWorld(Vector3(100, -5, 0), 30, 0);
	obj->GetPhysicsObject()->SetElasticity(1.0f);
	obj->GetRenderObject()->SetColour(Vector4(0.5, 0.3, 0.2, 1));
	obj = AddSphereToWorld(Vector3(-100, -5, 0), 30, 0);
	obj->GetPhysicsObject()->SetElasticity(1.0f);
	obj->GetRenderObject()->SetColour(Vector4(0.5, 0.3, 0.2, 1));*/

	obj = AddBouncerToWorld(Vector3(0, -5, 100), Vector3(0, 0, -1), 30);
	obj->GetPhysicsObject()->SetElasticity(1.0f);
	obj->GetRenderObject()->SetColour(Vector4(0.5, 0.3, 0.2, 1));
	obj = AddBouncerToWorld(Vector3(0, -5, -100), Vector3(0, 0, 1), 30);
	obj->GetPhysicsObject()->SetElasticity(1.0f);
	obj->GetRenderObject()->SetColour(Vector4(0.5, 0.3, 0.2, 1));
	obj = AddBouncerToWorld(Vector3(100, -5, 0), Vector3(-1, 0, 0), 30);
	obj->GetPhysicsObject()->SetElasticity(1.0f);
	obj->GetRenderObject()->SetColour(Vector4(0.5, 0.3, 0.2, 1));
	obj = AddBouncerToWorld(Vector3(-100, -5, 0), Vector3(1, 0, 0), 30);
	obj->GetPhysicsObject()->SetElasticity(1.0f);
	obj->GetRenderObject()->SetColour(Vector4(0.5, 0.3, 0.2, 1));

	obj = AddCubeToWorld(Vector3(0, 10, 20), Vector3(14, 5, 1), 10);
	PositionAxisConstraint* constraint = new PositionAxisConstraint(obj, Vector3(1, 1, 0));
	world->AddConstraint(constraint);

	obj = AddCubeToWorld(Vector3(-30, 10, 40), Vector3(1, 5, 14), 10);
	constraint = new PositionAxisConstraint(obj, Vector3(0, 1, 1));
	world->AddConstraint(constraint);
	//AddCubeToWorld(Vector3(0, -5, 0), Vector3(100, 2, 100), 0);

	//AddCubeToWorld(Vector3(0, 0, 0), Vector3(5, 20, 100), 0);


}

void GameA::BridgeConstraintTest() {


	Vector3 cubeSize = Vector3(8, 8, 8);

	float invCubeMass = 5; //how heavy the middle pieces are
	int numLinks = 10;
	float maxDistance = 30; // constraint distance
	float maxAngle = 0; // constraint angle
	float cubeDistance = 20; // distance between links

	Vector3 startPos = Vector3(500, 500, 500);

	GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0)
		, cubeSize, 0);
	GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks + 2)
		* cubeDistance, 0, 0), cubeSize, 0);

	GameObject* previous = start;

	for (int i = 0; i < numLinks; ++i) {
		GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) *
			cubeDistance, 0, 0), cubeSize, invCubeMass);
		PositionConstraint* constraint1 = new PositionConstraint(previous,
			block, maxDistance);
		world->AddConstraint(constraint1);

		RotationConstraint* constraint2 = new RotationConstraint(previous,
			block, maxAngle);
		world->AddConstraint(constraint2);

		previous = block;

	}
	PositionConstraint* constraint = new PositionConstraint(previous,
		end, maxDistance);
	world->AddConstraint(constraint);
	RotationConstraint* constraint2 = new RotationConstraint(previous,
		end, maxAngle);
	world->AddConstraint(constraint2);

}

/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* GameA::AddFloorToWorld(const Vector3& position) {
	GameObject* floor = new GameObject();

	Vector3 floorSize = Vector3(100, 2, 100);

	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	AABBVolume* volume = new AABBVolume(floor->GetTransform());
	floor->SetBoundingVolume((CollisionVolume*)volume);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(floor);

	return floor;
}

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple'
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* GameA::AddSphereToWorld(const Vector3& position, float radius, float inverseMass, bool clipping) {
	GameObject* sphere = new GameObject("Sphere");

	Vector3 sphereSize = Vector3(radius, radius, radius);


	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	SphereVolume* volume = new SphereVolume(sphere->GetTransform());
	volume->SetClipping(clipping);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* GameA::AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass) {
	GameObject* capsule = new GameObject("Capsule");


	capsule->GetTransform()
		.SetScale(Vector3(radius * 2, halfHeight, radius * 2))
		.SetPosition(position);

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius, capsule->GetTransform());
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTex, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	capsule->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(capsule);



	return capsule;

}

GameObject* GameA::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, bool clipping) {
	GameObject* cube = new GameObject();



	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	AABBVolume* volume = new AABBVolume(cube->GetTransform());
	volume->SetClipping(clipping);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

GameObject* NCL::CSC8503::GameA::AddIceToWorld(const Vector3& position, Vector3 dimensions, float inverseMass)
{
	GameObject* stuff = new Ice();

	stuff->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	AABBVolume* volume = new AABBVolume(stuff->GetTransform());

	stuff->SetBoundingVolume((CollisionVolume*)volume);
	volume->SetClipping(true);
	stuff->SetRenderObject(new RenderObject(&stuff->GetTransform(), cubeMesh, basicTex, basicShader));
	stuff->SetPhysicsObject(new PhysicsObject(&stuff->GetTransform(), stuff->GetBoundingVolume()));

	stuff->GetPhysicsObject()->SetInverseMass(inverseMass);
	stuff->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(stuff);

	return stuff;
}

GameObject* NCL::CSC8503::GameA::AddBouncerToWorld(const Vector3& position, Vector3 axis, float size)
{
	GameObject* bouncer = new Bouncer(axis,  position, 3, 2000);

	Vector3 sphereSize = Vector3(size, size, size);


	bouncer->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	SphereVolume* volume = new SphereVolume(bouncer->GetTransform());
	volume->SetClipping(true);
	bouncer->SetBoundingVolume((CollisionVolume*)volume);

	bouncer->SetRenderObject(new RenderObject(&bouncer->GetTransform(), sphereMesh, basicTex, basicShader));
	bouncer->SetPhysicsObject(new PhysicsObject(&bouncer->GetTransform(), bouncer->GetBoundingVolume()));

	bouncer->GetPhysicsObject()->SetInverseMass(1);
	bouncer->GetPhysicsObject()->InitSphereInertia();
	bouncer->GetPhysicsObject()->SetDamping(10);
	world->AddGameObject(bouncer);

	VelocityAxisConstraint* constraint = new VelocityAxisConstraint(bouncer,
		axis);
	world->AddConstraint(constraint);
	
	return bouncer;
}

GameObject* GameA::AddOBBToWorld(const Vector3& position, Vector3 rotation, Vector3 dimensions, float inverseMass) {
	GameObject* cube = new GameObject();

	cube->GetTransform()
		.SetPosition(position)
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(rotation.x, rotation.y, rotation.z))
		.SetScale(dimensions * 2);

	OBBVolume* volume = new OBBVolume(cube->GetTransform());
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

void GameA::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(position, radius, 1.0f);
		}
	}
	AddFloorToWorld(Vector3(0, -2, 0));
}

void GameA::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);

			if (rand() % 2) {
				cubeDims.y = rand() % 3 + 0.2f;
				GameObject* rect = AddCubeToWorld(position, cubeDims);
				//PositionAxisConstraint* constraint = new PositionAxisConstraint(rect,
				//	Vector3(1,0,0));
				//world->AddConstraint(constraint);
				rect->GetTransform().SetScale(Vector3(1, 1, 1));
			}
			else {
				GameObject* sphere = AddSphereToWorld(position, sphereRadius);
				sphere->GetTransform().SetScale(Vector3(3, 3, 3));
			}
		}
	}
}

void GameA::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 1; x < numCols + 1; ++x) {
		for (int z = 1; z < numRows + 1; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddCubeToWorld(position, cubeDims, 1.0f);
		}
	}
}

void GameA::InitDefaultFloor() {
	AddFloorToWorld(Vector3(0, -2, 0));
}

void GameA::InitGameExamples() {
	AddPlayerToWorld(Vector3(0, 5, 0));
	AddEnemyToWorld(Vector3(5, 5, 0));
	//AddBonusToWorld(Vector3(10, 5, 0));
}

/*GameObject* GameA::AddPlayerToWorld(const Vector3& position) {
	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject();



	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	AABBVolume* volume = new AABBVolume(character->GetTransform());
	character->SetBoundingVolume((CollisionVolume*)volume);

	if (rand() % 2) {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshA, nullptr, basicShader));
	}
	else {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshB, nullptr, basicShader));
	}
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	//lockedObject = character;

	return character;
}*/

GameObject* GameA::AddEnemyToWorld(const Vector3& position) {
	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject();



	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	AABBVolume* volume = new AABBVolume(character->GetTransform());
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	return character;
}

GameObject* NCL::CSC8503::GameA::AddPointToWorld(const Vector3& position)
{
	GameObject* bonus = new PointToken(world, score);


	bonus->GetTransform()
		.SetScale(Vector3(3, 3, 3))
		.SetPosition(position);

	SphereVolume* volume = new SphereVolume(bonus->GetTransform());
	volume->SetClipping(true);
	bonus->SetBoundingVolume((CollisionVolume*)volume);

	bonus->SetRenderObject(new RenderObject(&bonus->GetTransform(), sphereMesh, nullptr, basicShader));
	bonus->SetPhysicsObject(new PhysicsObject(&bonus->GetTransform(), bonus->GetBoundingVolume()));
	bonus->GetRenderObject()->SetColour(Vector4(1, 1, 0, 0.6));
	bonus->GetPhysicsObject()->SetInverseMass(0.0f);
	bonus->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(bonus);

	return bonus;
}

/*GameObject* GameA::AddBonusToWorld(const Vector3& position) {
	GameObject* bonus = new PowerUp(PowerUpType::Bonus, this);


	bonus->GetTransform()
		.SetScale(Vector3(0.25, 0.25, 0.25))
		.SetPosition(position);

	SphereVolume* volume = new SphereVolume(bonus->GetTransform());
	bonus->SetBoundingVolume((CollisionVolume*)volume);

	bonus->SetRenderObject(new RenderObject(&bonus->GetTransform(), bonusMesh, nullptr, basicShader));
	bonus->SetPhysicsObject(new PhysicsObject(&bonus->GetTransform(), bonus->GetBoundingVolume()));

	bonus->GetPhysicsObject()->SetInverseMass(1.0f);
	bonus->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(bonus);

	return bonus;
}*/

StateGameObject* NCL::CSC8503::GameA::AddStateObjectToWorld(const Vector3& position)
{
	StateGameObject* apple = new StateGameObject();


	apple->GetTransform()
		.SetScale(Vector3(0.25, 0.25, 0.25))
		.SetPosition(position);

	SphereVolume* volume = new SphereVolume(apple->GetTransform());
	apple->SetBoundingVolume((CollisionVolume*)volume);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonusMesh, nullptr, basicShader));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(1.0f);
	apple->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(apple);

	return apple;
}

GameObject* NCL::CSC8503::GameA::AddPlayerToWorld(const Vector3& position)
{
	player = new PlayerBall();


	player->GetTransform()
		.SetScale(Vector3(3, 3, 3))
		.SetPosition(position);

	SphereVolume* volume = new SphereVolume(player->GetTransform());
	player->SetBoundingVolume((CollisionVolume*)volume);

	player->SetRenderObject(new RenderObject(&player->GetTransform(), sphereMesh, nullptr, basicShader));
	player->SetPhysicsObject(new PhysicsObject(&player->GetTransform(), player->GetBoundingVolume()));

	player->GetPhysicsObject()->SetInverseMass(10.0f);
	player->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(player);

	return player;
}

/*

Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around.

*/
bool GameA::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		renderer->DrawString("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;
				lockedObject = nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;
				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		renderer->DrawString("Press Q to change to select mode!", Vector2(5, 85));
	}

	if (lockedObject) {
		renderer->DrawString("Press L to unlock object!", Vector2(5, 80));
	}

	else if (selectionObject) {
		renderer->DrawString("Press L to lock selected object object!", Vector2(5, 80));
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L)) {
		if (selectionObject) {
			if (lockedObject == selectionObject) {
				lockedObject = nullptr;
			}
			else {
				lockedObject = selectionObject;
			}
		}

	}

	if (selectionObject && Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::Z)) {

		//fire raycast from objects forward position
		//std::cout << "Firing Ray";
		//Debug::DrawLine(selectionObject->GetTransform().GetPosition(), selectionObject->GetTransform().GetOrientation().ToEuler() * 500 + selectionObject->GetTransform().GetPosition(), Vector4(0, 1, 0, 1), 10.0f);
		Ray ray = Ray(selectionObject->GetTransform().GetPosition(), selectionObject->GetTransform().GetOrientation() * Vector3(0, 0, -100));
		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			auto object = (GameObject*)closestCollision.node;
			std::cout << "Hit: " << object->GetName() << "\n";
		}
		//Debug::DrawLine(selectionObject->GetTransform().GetPosition(), selectionObject->GetTransform().GetOrientation() * Vector3(0, 0, -100) + selectionObject->GetTransform().GetPosition(), Vector4(0, 1, 0, 1), 10.0f);

	}

	return false;
}

bool NCL::CSC8503::GameA::DebugObjects()
{
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT)) {

		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {

			GameObject* clicked = (GameObject*)closestCollision.node;
			debugText = clicked->OnDebug();
		}
		else
		{
			debugText = "";
		}
	}
	return false;
}

bool NCL::CSC8503::GameA::ClickObjects()
{
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::LEFT)) {

		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {

			GameObject* clicked = (GameObject*)closestCollision.node;
			clicked->OnClick(clickText);
		}
		else
		{
			clickText = "";
		}


	}
	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/
void GameA::MoveSelectedObject() {
	renderer->DrawString("Click Force:" + std::to_string(forceMagnitude),
		Vector2(10, 20)); //Draw debug text at 10,20
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

	if (!selectionObject) {
		return;//we haven’t selected anything!

	}
	//Push the selected object!
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(
			*world->GetMainCamera());
		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			/*if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->
					AddForce(ray.GetDirection() * forceMagnitude);
			}*/
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->AddForceAtPosition(
					ray.GetDirection() * forceMagnitude,
					closestCollision.collidedAt);

			}
		}
	}
}