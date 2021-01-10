#include "Item.h"
#include "Collisions.h"
#include "Textures.h"
#include "Animation.h"
#include "Scene.h"
#include "Audio.h"
#include "EntityManager.h"

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
	checked = false;
	checkCheck = 0;

	pickCoin = app->scene->pickCoin;
	pickHeart = app->scene->pickHeart;
	checkpoint = app->scene->pickCheckpoint;

	if (type == ItemType::HEART)
	{
		collider = app->collisions->AddCollider(rCollider, COLLIDER_HEART, nullptr, this);
	}
	if (type == ItemType::COIN)
	{
		collider = app->collisions->AddCollider(rCollider, COLLIDER_COIN, nullptr, this);
	}
	if (type == ItemType::CHECKPOINT)
	{
		collider = app->collisions->AddCollider({rCollider.x,rCollider.y,18,80}, COLLIDER_CHECKPOINT, nullptr, this);
	}
	
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

	checkOFF.PushBack({ 0, 40, 20, 62 });
	checkON.PushBack({ 20, 40, 20, 62 });

	if (type == ItemType::HEART)
	{
		currentAnimation = &idleHeart;
	}
	else if (type == ItemType::COIN)
	{
		currentAnimation = &idleCoin;
	}
	else if (type == ItemType::CHECKPOINT)
	{
		currentAnimation = &checkOFF;
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
	if (active && app->scene->player->godMode == false)
	{
		if (c1 == collider && c2->type == COLLIDER_PLAYER)
		{
			if (active)
			{
				if (collider->type == ColliderType::COLLIDER_HEART)
				{
					app->audio->PlayFx(pickHeart);
					app->scene->player->lives++;
					app->scene->player->life = 100;
					active = false;
					ret = CleanUp();
				}
				else if (collider->type == ColliderType::COLLIDER_COIN)
				{
					app->audio->PlayFx(pickCoin);
					app->scene->player->money++;
					active = false;
					ret = CleanUp();
				}
				else if (collider->type == ColliderType::COLLIDER_CHECKPOINT)
				{
					if(checkCheck == 0) app->audio->PlayFx(checkpoint);
					checkCheck = 1;
					currentAnimation = &checkON;
					checked = true;
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
	/*app->audio->UnloadFx(pickCoin);
	app->audio->UnloadFx(pickHeart);
	app->audio->UnloadFx(checkpoint);*/
	if (collider != nullptr)
	{
		collider->toDelete = true;
		collider = nullptr;
	}
	return ret;
}