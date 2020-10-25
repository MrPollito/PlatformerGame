#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
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

void Map::Draw()
{
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	MapLayer* layer = mapData.layers.start->data;

	for (int y = 0; y < mapData.height; ++y)
	{
		for (int x = 0; x < mapData.width; ++x)
		{
			int tileId = layer->Get(x, y);
			if (tileId > 0)
			{
				// L04: TODO 9: Complete the draw function
				SDL_Rect rect = mapData.tilesets.start->data->GetTileRect(tileId);
				iPoint position = MapToWorld(x, y);
				app->render->DrawTexture(mapData.tilesets.start->data->texture, position.x, position.y, &rect);


			}
		}
	}

}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
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

	// L04: DONE 7: Get relative Tile rectangle
	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));

	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// L03: TODO 2: Make sure you clean up any memory allocated from tilesets/map
	// Remove all tilesets
	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}

	mapData.tilesets.clear();

	// L04: DONE 2: clean up all layer data
    // Remove all layers

	ListItem<MapLayer*>* item2;
	item2 = mapData.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	mapData.layers.clear();

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
		TileSet* sets = new TileSet();

		if (ret == true) ret = LoadTileSetDetails(tiledata, sets);

		if (ret == true) ret = LoadTileSetImage(tiledata, sets);

		if (ret == true) mapData.tilesets.add(sets);
	}

	// L04: DONE 4: Iterate all layers and load each of them
	// Load layer info
	for (pugi::xml_node layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();
		if (ret == true)
		{
			ret = LoadLayer(layer, set);
		}
		mapData.layers.add(set);
	}

	if (ret == true)
	{
		// L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
		LOG("Successfully parsed map XML file : %s", loadingLevel.GetString());
		LOG(" width : %d height : %d", mapData.width, mapData.height);
		LOG(" tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

		// LOG tilesets
		ListItem<TileSet*>* item = mapData.tilesets.end;

		while (item != NULL)
		{
			LOG("Tileset----");
			LOG("name : %s firstgid : %d", item->data->name.GetString(), item->data->firstgid);
			LOG("tile width : %d tile height : %d", item->data->tileWidth, item->data->tileHeight);
			LOG("spacing : %d margin : %d", item->data->spacing, item->data->margin);
			item = item->prev;
		}
		// L04: TODO 4: LOG the info for each loaded layer
		// Log layers
		ListItem<MapLayer*>* item2 = mapData.layers.start;
	
		while (item2 != NULL)
		{
			LOG("Layer----");
			LOG("Name = %s", item2->data->name.GetString());
			LOG("Width = %d Height = %d", item2->data->width, item2->data->height);
			item2 = item2->next;
		}

	}

	mapLoaded = ret;

	return ret;
}

bool Map::LoadMap()
{
	bool ret = true;
	SString path = folder.GetString();
	path += loadingLevel.GetString();
	pugi::xml_node map;
	pugi::xml_parse_result result = mapFile.load_file(path.GetString());

	if (result != NULL)
	{
		map = mapFile.child("map");
		mapData.height = map.attribute("height").as_int();
		mapData.width = map.attribute("width").as_int();
		mapData.orientation = map.attribute("orientation").as_string();
		mapData.tileHeight = map.attribute("tileheight").as_int();
		mapData.tileWidth = map.attribute("tilewidth").as_int();

		LOG("Map loaded correctly!");
		mapLoaded = true;
		ret = true;
	}
	else
	{
		LOG("Fail loading the tmx file. %s", result.description());
	}
	return ret;
}

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

	return ret;
}

bool Map::LoadTileSetImage(pugi::xml_node& node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load Tileset image
		SString path = folder;
		path += node.child("image").attribute("source").as_string();
		set->texture = app->tex->Load(path.GetString());
		set->imageWitdth = node.child("image").attribute("width").as_int();
		set->imageHeight = node.child("image").attribute("height").as_int();
	}

	return true;
}


// L04: TODO 3: Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	// L04: TODO 3: Load a single layer
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	layer->data = new uint[(mapData.width * mapData.height * sizeof(uint))];
	memset(layer->data, 0, size_t(mapData.width * mapData.height * sizeof(uint)));

	pugi::xml_node tileNode;

	int i = 0;
	for (tileNode = node.child("data").child("tile"); tileNode && ret; tileNode = tileNode.next_sibling("tile"))
	{
		if (ret == true)
		{
			layer->data[i] = node.attribute("gid").as_uint();
		}

		i++;
	}

	LOG("Layer <<%s>> has loaded %d tiles", layer->name.GetString(), i);
	return ret;
}
