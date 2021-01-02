#include "Item.h"
#include "Collisions.h"
#include "Textures.h"
#include "Animation.h"
#include "Scene.h"

Item::Item(int x, int y, ItemType type) : Entity(EntityType::ITEM)
{
	type = type;
	active = true;
	position.x = x;
	position.y = y;
	rCollider.x = position.x;
	rCollider.y = position.y;
	rCollider.w = 18;
	rCollider.h = 18;
	r.x = position.x;
	r.y = position.y;

	collider = app->collisions->AddCollider(rCollider, COLLIDER_ITEM, nullptr, this);
	itemTexture = app->tex->Load("Assets/textures/Items.png");

	for (int i = 0; i < 5; i++)
	{
		idleHeart.PushBack({ (18 * i),18,18,18 });
	}
	idleHeart.speed = 0.2f;

	for (int i = 0; i < 6; i++)
	{
		idleCoin.PushBack({ (18 * i),0,18,18 });
	}
	idleCoin.speed = 0.2f;

	if (type == ItemType::HEART)
	{
		currentAnimation = &idleHeart;
	}
	else if (type == ItemType::COIN)
	{
		currentAnimation = &idleCoin;
	}
}

Item::~Item()
{
}

bool Item::Update(float dt)
{
	bool ret = false;
	if (active) ret = Draw(dt);
	return true;
}

bool Item::Draw(float dt)
{
	bool ret = false;
	r = currentAnimation->GetCurrentFrame(dt);

	if (itemTexture != nullptr)
	{
			ret = app->render->DrawTexture(itemTexture, position.x, position.y, &r, false, 1, 1, INT_MAX, INT_MAX);
	}

	r.x = position.x;
	r.y = position.y;
	return ret;
}

bool Item::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = false;
	if (active)
	{
		if (c1 == collider && c2->type == COLLIDER_PLAYER)
		{
			if (active)
			{
				if (type == ItemType::HEART)
				{
					app->scene->player->lives++;
					active = false;
					ret = CleanUp();
				}
				else if (type == ItemType::COIN)
				{
					app->scene->player->money++;
					active = false;
					ret = CleanUp();
				}
			}
		}
	}
	else ret = true;
	return ret;
}

bool Item::CleanUp()
{
	bool ret = false;
	ret = app->tex->UnLoad(itemTexture);
	if (collider != nullptr)
	{
		collider->toDelete = true;
		collider = nullptr;
	}
	return ret;
}