#include "EntityManager.h"

#include "App.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Scene.h"
#include "Collisions.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;


	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	ListItem<Entity*>* enti = entities.start;
	while (enti != NULL)
	{
		enti->data->CleanUp();
		enti = enti->next;
	}

	return true;
}

Entity* EntityManager::CreateEntity(EntityType type, int x, int y, ItemType itemType)
{
	Entity* ret = nullptr;

	switch (type)
	{
	case EntityType::PLAYER: ret = new Player();  break;
	case EntityType::PIG_ENEMY: ret = new PigEnemy();  break;
	case EntityType::ITEM: ret = new Item(x,y,itemType);  break;
	default: break;
	}

	if (ret != nullptr) entities.Add(ret);

	return ret;
}


bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool EntityManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic)
	{
		//// TODO: Update all entities 
		ListItem<Entity*>* enti = entities.start;
		while (enti != NULL)
		{
			enti->data->Update(dt);
			enti = enti->next;
		}
	}

	return true;
}
