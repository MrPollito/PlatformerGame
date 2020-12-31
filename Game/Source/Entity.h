#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"

enum class EntityType
{
    PLAYER,
    ENEMY,
    ITEM,
    MAP,
    UNKNOWN
};

struct Collider;

class Entity
{
public:

    Entity(EntityType type) : type(type), active(true) {}

    virtual bool Awake(pugi::xml_node&)
    {
        return true;
    }

    virtual bool Update(float dt)
    {
        return true;
    }

    virtual bool OnCollision(Collider* c1, Collider* c2)
    {
        return true;
    }

public:

    EntityType type;
    bool active = true;
    SString name;
    //SString name;         // Entity name identifier?
    //uint32 id;            // Entity identifier?

    // Possible properties, it depends on how generic we
    // want our Entity class, maybe it's not renderable...
    iPoint position;        // Use a float instead?
    bool renderable = false;
    //SDL_Texture* texture;
};

#endif // __ENTITY_H__