#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
struct Collider;

class Module
{
public:

	Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool PreUpdate()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&)
	{
		return true;
	}

	virtual bool OnCollision(Collider* c1, Collider* c2)
	{
		return true;
	}

public:

	SString name;
	bool active;

};

#endif // __MODULE_H__