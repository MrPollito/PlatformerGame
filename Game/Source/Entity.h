#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Collisions.h"

enum class EntityType
{
    PLAYER,
    PIG_ENEMY,
    BAT_ENEMY,
    ITEM,
    UNKNOWN
};

class Entity
{
public:

    Entity(EntityType type) : type(type), active(true) {}

    virtual bool Update(float dt)
    {
        return true;
    }

    virtual bool OnCollision(Collider* c1, Collider* c2)
    {
        return true;
    }

    virtual bool CleanUp()
    {
        return true;
    }
    
    virtual bool Load(pugi::xml_node& file)
    {
        return true;
    }

    virtual bool Save(pugi::xml_node& file)
    {
        return true;
    }

public:

    EntityType type;
    bool active = true;
    bool deleted = false;
    SString name;
};

#endif // __ENTITY_H__