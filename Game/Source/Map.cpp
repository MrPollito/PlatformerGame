
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Collisions.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
	name.Create("map");

}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.Create(config.child("folder").child_value());
	loadingLevel.Create(config.child("level").child_value());

	return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	// L04: TODO 5: Prepare the loop to draw all tilesets + DrawTexture()
	ListItem<MapLayer*>* layer = mapData.layers.start;

	while (layer != NULL)
	{
		for (int y = 0; y < mapData.height; ++y)
		{
			for (int x = 0; x < mapData.width; ++x)
			{

				int tileId = layer->data->Get(x, y);
				if (tileId > 0)
				{
					// L04: TODO 9: Complete the draw function
					SDL_Rect r = mapData.tilesets.start->data->GetTileRect(tileId);
					iPoint pos = MapToWorld(x, y);
					app->render->DrawTexture(mapData.tilesets.start->data->texture, pos.x, pos.y, &r);

				}

			}
		}

		layer = layer->next;
	}

}

// L04: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * mapData.tileWidth;
	ret.y = y * mapData.tileHeight;

	return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	// L04: TODO 7: Get relative Tile rectangle
	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % (imageWitdth / tileWidth)));
	rect.y = margin + ((rect.h + spacing) * (relativeId / (imageWitdth / tileWidth)));

	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// L03: TODO 2: Make sure you clean up any memory allocated from tilesets/map
	//Delete list of TileSets---------------------
	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.clear();

	// L04: TODO 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = mapData.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	mapData.layers.clear();

	// Clean up the pugui tree
	mapFile.reset();

	return true;
}

// Load new map
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

	// Load general info
	if (ret == true)
	{
		// L03: TODO 3: Create and call a private function to load and fill all your map data
		ret = LoadMap();
	}

	// L03: TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	pugi::xml_node tiledata;
	for (tiledata = mapFile.child("map").child("tileset"); tiledata && ret; tiledata = tiledata.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTileSetDetails(tiledata, set);

		if (ret == true) ret = LoadTileSetImage(tiledata, set);

		mapData.tilesets.add(set);
	}

	// L04: TODO 4: Iterate all layers and load each of them
	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true) mapData.layers.add(lay);

	}

	if (ret == true)
	{
		// L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
		LOG("Successfully parsed map XML file : %s", loadingLevel.GetString());
		LOG(" width : %d height : %d", mapData.width, mapData.height);
		LOG(" tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

		ListItem<TileSet*>* item = mapData.tilesets.start;

		while (item != NULL)
		{
			LOG("Tileset----");
			LOG("name : %s firstgid : %d", item->data->name.GetString(), item->data->firstgid);
			LOG("tile width : %d tile height : %d", item->data->tileWidth, item->data->tileHeight);
			LOG("spacing : %d margin : %d", item->data->spacing, item->data->margin);
			item = item->next;
		}

		// L04: TODO 4: LOG the info for each loaded layer
		ListItem<MapLayer*>* item2 = mapData.layers.start;

		while (item2 != NULL)
		{
			LOG("Layer----");
			LOG("name : %s ", item2->data->name.GetString());
			LOG("width : %d height : %d", item2->data->width, item2->data->height);
			LOG("Tiles in Layer %s ----", item2->data->name.GetString());
			for (int i = 0; i < (item2->data->width * item2->data->height * sizeof(uint)); i++)
			{
				LOG("tile gid: %d", item2->data->data[i]);
			}

			item2 = item2->next;
		}
	}

	mapLoaded = ret;

	return ret;
}

bool Map::LoadMap()
{
	bool ret = false;
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
		// L03: TODO: Load map general properties
		pugi::xml_node map = mapFile.child("map");
		mapData.height = map.attribute("height").as_int();
		mapData.width = map.attribute("width").as_int();
		mapData.orientation = map.attribute("orientation").as_string();
		mapData.tileHeight = map.attribute("tileheight").as_int();
		mapData.tileWidth = map.attribute("tilewidth").as_int();

		LOG("Map loaded correctly!");

		mapLoaded = true;
		ret = true;
	}

	return ret;
}

// L03: TODO: Load Tileset attributes
bool Map::LoadTileSetDetails(pugi::xml_node& node, TileSet* set)
{
	bool ret = true;

	// L03: TODO: Load Tileset attributes
	set->name.Create(node.attribute("name").as_string());
	set->tileWidth = node.attribute("tilewidth").as_int();
	set->tileHeight = node.attribute("tileheight").as_int();
	set->firstgid = node.attribute("firstgid").as_int();
	set->spacing = node.attribute("spacing").as_int();
	set->margin = node.attribute("margin").as_int();

	return true;
}

// L03: TODO: Load Tileset image
bool Map::LoadTileSetImage(pugi::xml_node& node, TileSet* set)
{
	SString path = folder;
	path += node.child("image").attribute("source").as_string();

	set->texture = app->tex->Load(path.GetString());
	set->imageWitdth = node.child("image").attribute("width").as_int();
	set->imageHeight = node.child("image").attribute("height").as_int();

	return true;
}

// L04: TODO 3: Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	layer->id = node.attribute("id").as_int();
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
		if (layer->GetId() == 2)
		{
			LOG("Calling Assign Colliders");
			AssignColliders(layer);
		}

	}


	return ret;
}

bool Map::AssignColliders(MapLayer* lay)
{
	for (int y = 0; y < mapData.height; ++y)
	{
		for (int x = 0; x < mapData.width; ++x)
		{
			uint tileId = lay->Get(x, y);
			if (tileId > 0)
			{

				SDL_Rect r = mapData.tilesets.start->data->GetTileRect(tileId);
				iPoint pos = MapToWorld(x, y);
				app->collisions->AddCollider(r, Collider::Type::WALL, this);

			}
		}
	}



	return true;
}

