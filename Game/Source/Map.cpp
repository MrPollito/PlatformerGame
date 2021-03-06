#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Collisions.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
	name.Create("map");
}

Map::~Map()
{}

bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.Create(config.child("folder").child_value());
	loadingLevel.Create(config.child("level").child_value());

	return ret;
}

void Map::Draw()
{
	if (mapLoaded == false)
	{
		return;
	}

	ListItem<MapLayer*>* layer = mapData.layers.start;
	TileSet* tile;

	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Draw") == 1)
		{

			for (int y = 0; y < mapData.height; ++y)
			{
				for (int x = 0; x < mapData.width; ++x)
				{

					int tileId = layer->data->Get(x, y);
					if (tileId > 0)
					{
						tile = GetTilesetFromTileId(tileId);
						SDL_Rect r = tile->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						app->render->DrawTexture(tile->texture, pos.x, pos.y, &r);

					}
				}
			}
		}

		layer = layer->next;
	}
}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * mapData.tileWidth;
	ret.y = y * mapData.tileHeight;

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret;

	ret.x = x / mapData.tileWidth;
	ret.y = y / mapData.tileHeight;

	return ret;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = mapData.tilesets.end;
	TileSet* set = item->data;

	while (item != NULL)
	{
		if (set->firstgid <= id)
		{
			return set;
		}
		item = item->prev;
		set = item->data;
	}

	return set;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % (imageWitdth / tileWidth)));
	rect.y = margin + ((rect.h + spacing) * (relativeId / (imageWitdth / tileWidth)));

	return rect;
}

bool Map::CleanUp()
{
	LOG("Unloading map");

	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

	ListItem<MapLayer*>* item2;
	item2 = mapData.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	mapData.layers.Clear();

	mapFile.reset();

	return true;
}

bool Map::Load(const char* filename)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	if (ret == true)
	{
		ret = LoadMap();

		pugi::xml_node tiledata;
		for (tiledata = mapFile.child("map").child("tileset"); tiledata && ret; tiledata = tiledata.next_sibling("tileset"))
		{
			TileSet* set = new TileSet();

			if (ret == true) ret = LoadTileSetDetails(tiledata, set);

			if (ret == true) ret = LoadTileSetImage(tiledata, set);

			if (ret == true) ret = LoadTileSetProperties(tiledata, set);

			mapData.tilesets.Add(set);
		}

		for (pugi::xml_node layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
		{
			MapLayer* set = new MapLayer();
			if (ret == true)
			{
				ret = LoadLayer(layer, set);
			}
			mapData.layers.Add(set);
		}
		LogAll();
	}

	mapLoaded = ret;

	return ret;
}

bool Map::LoadMap()
{
	bool ret = true;
	SString path = folder.GetString();
	path += loadingLevel.GetString();
	pugi::xml_parse_result result = mapFile.load_file(path.GetString());

	if (result == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		pugi::xml_node map = mapFile.child("map");
		mapData.height = map.attribute("height").as_int();
		mapData.width = map.attribute("width").as_int();
		mapData.orientation = map.attribute("orientation").as_string();
		mapData.tileHeight = map.attribute("tileheight").as_int();
		mapData.tileWidth = map.attribute("tilewidth").as_int();

		LOG("Map loaded correctly!");
	}

	return ret;
}

bool Map::LoadTileSetDetails(pugi::xml_node& node, TileSet* set)
{
	bool ret = true;

	set->name.Create(node.attribute("name").as_string());
	set->tileWidth = node.attribute("tilewidth").as_int();
	set->tileHeight = node.attribute("tileheight").as_int();
	set->firstgid = node.attribute("firstgid").as_int();
	set->spacing = node.attribute("spacing").as_int();
	set->margin = node.attribute("margin").as_int();

	return true;
}

bool Map::LoadTileSetImage(pugi::xml_node& node, TileSet* set)
{
	SString path = folder;
	path += node.child("image").attribute("source").as_string();

	set->texture = app->tex->Load(path.GetString());
	set->imageWitdth = node.child("image").attribute("width").as_int();
	set->imageHeight = node.child("image").attribute("height").as_int();

	return true;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();

	layer->data = new uint[layer->width * layer->height * sizeof(uint)];
	memset(layer->data, 0, layer->width * layer->height * sizeof(uint));

	pugi::xml_node layerNode = node.child("data");

	if (layerNode == NULL)
	{
		LOG("Error loading node child data, inside LoadMapLayers");
		ret = false;
	}
	else
	{
		int i = 0;
		for (pugi::xml_node tile = layerNode.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i] = tile.attribute("gid").as_int();
			i++;
		}
		if (strcmp(layer->name.GetString(), "collisions") == 0)
		{
			AssignColliders(layer);
		}
	}
	LOG("Loading layer properties");

	ret = LoadProperties(node.child("properties"), layer->properties);
	return ret;
}

void Map::AssignColliders(MapLayer* lay)
{
	for (int y = 0; y < mapData.height; ++y)
	{
		for (int x = 0; x < mapData.width; ++x)
		{
			uint tileId = lay->Get(x, y);
			if (tileId > 0)
			{
				TileSet* set = mapData.tilesets.start->data;
				SDL_Rect r = set->GetTileRect(tileId);
				iPoint pos = MapToWorld(x, y);
				r.x = pos.x;
				r.y = pos.y;
				app->collisions->AddCollider(r, ColliderType::COLLIDER_GROUND, this, nullptr);

			}
		}
	}
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;
	pugi::xml_node property;
	for (property = node.child("property"); property; property = property.next_sibling("property"))
	{
		Properties::Property* prop = new Properties::Property();

		prop->name = property.attribute("name").as_string();
		prop->value = property.attribute("value").as_int();

		properties.list.Add(prop);
	}

	return ret;
}

bool Map::LoadTileSetProperties(pugi::xml_node& node, TileSet* set)
{
	bool ret = true;
	for (pugi::xml_node tileNode = node.child("tile"); tileNode && ret; tileNode = tileNode.next_sibling("tile"))
	{
		Tile* tileProperties = new Tile;
		tileProperties->id = tileNode.attribute("id").as_int();
		ret = LoadProperties(tileNode.child("properties"), tileProperties->properties);
		set->tileSetPropList.Add(tileProperties);
	}
	return ret;
}

int Properties::GetProperty(const char* value, int defaultValue) const
{
	ListItem<Property*>* property;
	property = list.start;

	SString prop = value;

	while (property != NULL)
	{
		if (property->data->name == prop)
		{
			return property->data->value;
		}
		property = property->next;
	}

	return defaultValue;
}

Tile* TileSet::GetPropList(int id) const
{
	ListItem<Tile*>* tile = tileSetPropList.start;
	Tile* t = tile->data;
	while (tile != NULL)
	{
		t = tile->data;
		if (t->id == id)
		{
			return t;
		}
		tile = tile->next;
	}
	return t;
}

void Properties::SetProperty(const char* name, int value)
{
	ListItem<Property*>* property;
	property = list.start;

	SString prop = name;

	while (property != NULL)
	{
		if (property->data->name == prop)
		{
			property->data->value = value;
			return;
		}
		property = property->next;
	}
}

void Map::LogAll()
{
	LOG("Successfully parsed map XML file : %s", app->map->folder);
	LOG(" width : %d height : %d", mapData.width, mapData.height);
	LOG(" tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

	ListItem<TileSet*>* tileList = mapData.tilesets.start;

	while (tileList != NULL)
	{
		LOG("Tileset----");
		LOG("name : %s firstgid : %d", tileList->data->name.GetString(), tileList->data->firstgid);
		LOG("tile width : %d tile height : %d", tileList->data->tileWidth, tileList->data->tileHeight);
		LOG("spacing : %d margin : %d", tileList->data->spacing, tileList->data->margin);
		tileList = tileList->next;
	}

	ListItem<MapLayer*>* layerList = mapData.layers.start;

	while (layerList != NULL)
	{
		LOG("Layer----");
		LOG("name : %s ", layerList->data->name.GetString());
		LOG("width : %d height : %d", layerList->data->width, layerList->data->height);
		LOG("Tiles in Layer %s ----", layerList->data->name.GetString());

		layerList = layerList->next;
	}
	ListItem<Properties::Property*>* propList;
	propList = mapData.layers.start->data->properties.list.start;
	while (layerList != NULL)
	{
		LOG("#Layer");
		LOG("Name=%s", layerList->data->name.GetString());
		LOG("Width=%d", layerList->data->width);
		LOG("Height=%d", layerList->data->height);

		while (propList != NULL)
		{
			LOG("#Property");
			LOG("Name=%s", propList->data->name.GetString());
			LOG("Value=%d", propList->data->value);
			propList = propList->next;
		}

		LOG("\n");
		layerList = layerList->next;
	}
}

ListItem<MapLayer*>* MapLayer::GetLayer(SString name)
{
	ListItem<MapLayer*>* a;
	a = app->map->mapData.layers.start;
	while (a != NULL)
	{
		if (a->data->name == name)
		{
			return a;
		}
	}
}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* item = mapData.layers.start;

	for (item = mapData.layers.start; item != NULL || ret == false; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.GetProperty("Navigation", 0) == 1)
		{
			uchar* map = new uchar[layer->width * layer->height];
			memset(map, 1, layer->width * layer->height);

			for (int y = 0; y < mapData.height; ++y)
			{
				for (int x = 0; x < mapData.width; ++x)
				{
					int i = (y * layer->width) + x;

					int tile_id = layer->Get(x, y);
					map[i] = (layer->data[tile_id] == 24) == true ? 1 : 0;
				}
			}

			*buffer = map;
			width = mapData.width;
			height = mapData.height;
			ret = true;
		}
	}

	return ret;
}