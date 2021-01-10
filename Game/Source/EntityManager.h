#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "Item.h"

#include "List.h"

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node& file);
	bool Save(pugi::xml_node& file);

	// Additional methods
	Entity* CreateEntity(EntityType type, int x, int y, ItemType itemType);
	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	bool UpdateAll(float dt, bool doLogic);

public:

	List<Entity*> entities;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
	int pigIds = 0;
	int batIds = 0;
};

#endif // __ENTITYMANAGER_H__

