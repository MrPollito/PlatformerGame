#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"
#include "Point.h"
#include "Collisions.h"

#include "PugiXml\src\pugixml.hpp"

// L03: TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!

struct Properties
{
	struct Property
	{
		SString name;
		int value;
	};
	~Properties()
	{
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}
		list.Clear();
	}

	// L06: TODO 7: Method to ask for the value of a custom property
	int GetProperty(const char* name, int default_value = 0) const;
	void SetProperty(const char* name, int value);

	List<Property*> list;
};

struct Tile
{
	int id;
	Properties properties;
};

struct TileSet
{
	SString name;
	int firstgid;
	int tileWidth;
	int tileHeight;
	int spacing;
	int margin;

	SDL_Texture* texture;
	int imageWitdth;
	int imageHeight;
	int numTilesWidth;
	int numTilesHeight;
	int offsetX;
	int offsetY;

	List<Tile*> tileSetPropList;
	Tile* GetPropList(int id) const;

	// L04: TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;
};

// L03: TODO 1: We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// L04: TODO 1: Create a struct for the map layer
struct MapLayer
{
	SString name;
	int width;
	int height;
	uint* data;
	

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// L04: TODO 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		uint ret = data[y * width + x];
		return ret;
	}

	ListItem<MapLayer*>* GetLayer(SString name);
	
};

// L03: TODO 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	SString orientation;
	SDL_Color backgroundColor;
	MapTypes type;

	List<TileSet*> tilesets;

	// L04: TODO 2: Add a list/array of layers to the map
	List<MapLayer*> layers;
};

class Map : public Entity
{
public:

	Map();

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	SString GetLoadingLevel()const
	{
		return loadingLevel;
	}

private:
	// L03: Methods to load all required map data
	bool LoadMap();
	bool LoadTileSetDetails(pugi::xml_node& node, TileSet* set);
	bool LoadTileSetImage(pugi::xml_node& node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadTileSetProperties(pugi::xml_node& node, TileSet* set);
	void AssignColliders(MapLayer* layer);

	TileSet* GetTilesetFromTileId(int id) const;
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

public:

	// L03: DONE 1: Add your struct for map info
	MapData mapData;
	List<Collider*> mapColliders;
	void LogAll();
	pugi::xml_document mapFile;
	SString folder;
	SString loadingLevel;
	bool mapLoaded;
};

#endif // __MAP_H__