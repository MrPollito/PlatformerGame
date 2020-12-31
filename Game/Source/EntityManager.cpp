#include "EntityManager.h"

#include "App.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Scene.h"
#include "Map.h"

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
	if (!active) return true;

	return true;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* ret = nullptr;

	switch (type)
	{
		// L13: Create the corresponding type entity
	    case EntityType::PLAYER: ret = new Player();  break;
		case EntityType::MAP: ret = new Player();  break;
		//case EntityType::ENEMY: ret = new Enemy();  break;
		//case EntityType::ITEM: ret = new Item();  break;
	default: break;
	}

	// Created entities are added to the list
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
