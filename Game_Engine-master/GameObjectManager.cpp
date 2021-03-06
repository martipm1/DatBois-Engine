#include "Application.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Imgui\imgui.h"
#include <vector>

GameObjectManager::GameObjectManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//root = new GameObject("Root_node", nullptr);
	root = nullptr;
}

GameObjectManager::~GameObjectManager()
{}

bool GameObjectManager::Start()
{
	return true;
}

update_status GameObjectManager::Update(float dt)
{
	if (root != nullptr)
	{
		if (root->active)
			root->Update();
	}

	return UPDATE_CONTINUE;
}

bool GameObjectManager::CleanUp()
{
	root->Delete();

	root = nullptr;

	return true;
}

GameObject* GameObjectManager::CreateGameObject(std::string name, GameObject* parent = nullptr)
{ 
	if (parent == nullptr)
		parent = root;

	GameObject* object = new GameObject(name, parent);

	return object;
}

void GameObjectManager::DeleteGameObject(GameObject* object)
{
	object->Delete();
}