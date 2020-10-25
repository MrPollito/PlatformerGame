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

	// L03: TODO 6: Iterate all tilesets and draw all their 

}


// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// L03: TODO 2: Make sure you clean up any memory allocated from tilesets/map

	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.clear();

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


	if (ret == true)
	{
		// L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
		LOG("Successfully parsed map XML file : %s", loadingLevel.GetString());
		LOG(" width : %d height : %d", mapData.width, mapData.height);
		LOG(" tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

		ListItem<TileSet*>* item = mapData.tilesets.end;

		while (item != NULL)
		{
			LOG("Tileset----");
			LOG("name : %s firstgid : %d", item->data->name.GetString(), item->data->firstGid);
			LOG("tile width : %d tile height : %d", item->data->tileWidth, item->data->tileHeight);
			LOG("spacing : %d margin : %d", item->data->spacing, item->data->margin);
			item = item->prev;
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
	set->firstGid = node.attribute("firstgid").as_int();
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

