#include "EntityManager.h"

#include "App.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Scene.h"
#include "Collisions.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
	pigIds = 0;
	batIds = 0;
}

// Destructor
EntityManager::~EntityManager()
{}


bool EntityManager::Start()
{
	LOG("start entityManager");
	bool ret = true;
	
	return ret;
}

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
	case EntityType::PIG_ENEMY:
	{
		pigIds++;
		ret = new PigEnemy(pigIds,x,y);
	}
	break;
	case EntityType::BAT_ENEMY:
	{
		batIds++;
		ret = new BatEnemy(batIds,x,y);
	}
	break;
	case EntityType::ITEM: ret = new Item(x,y,itemType);  break;
	default: break;
	}

	if (ret != nullptr) entities.Add(ret);

	return ret;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* enti = entities.start;
	while (enti != NULL)
	{
		SString tmpname = enti->data->name;
		if (tmpname == entity->name.GetString())
		{
			enti->data->CleanUp();
			enti->data->deleted = true;
			break;
		}
		else
		{
			enti = enti->next;
		}
	}
	entity->CleanUp();
	entity->deleted = true;
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

bool EntityManager::Load(pugi::xml_node& file)
{
	ListItem<Entity*>* enti = entities.start;
	while (enti != NULL)
	{
		if (enti->data->deleted == false)
		{
			enti->data->Load(file);
			enti = enti->next;
		}
		else
		{
			enti = enti->next;
		}
	}
	return true;
}

bool EntityManager::Save(pugi::xml_node& file)
{
	ListItem<Entity*>* enti = entities.start;
	while (enti != NULL)
	{
		if (enti->data->deleted == false)
		{
			enti->data->Save(file);
			enti = enti->next;
		}
		else
		{
			enti = enti->next;
		}
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
			if (enti->data->deleted == false)
			{
				enti->data->Update(dt);
				enti = enti->next;
			}
			else
			{
				enti = enti->next;
			}
		}
	}

	return true;
}
