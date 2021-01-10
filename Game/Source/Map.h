#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Collisions.h"

#include "PugiXml\src\pugixml.hpp"

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

	SDL_Rect GetTileRect(int id) const;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

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

	inline uint Get(int x, int y) const
	{
		uint ret = data[y * width + x];
		return ret;
	}

	ListItem<MapLayer*>* GetLayer(SString name);

};

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

	List<MapLayer*> layers;
};

class Map : public Module
{
public:

	Map();

	virtual ~Map();

	bool Awake(pugi::xml_node& conf);

	void Draw();

	bool CleanUp();

	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	SString GetLoadingLevel()const
	{
		return loadingLevel;
	}

private:

	bool LoadMap();
	bool LoadTileSetDetails(pugi::xml_node& node, TileSet* set);
	bool LoadTileSetImage(pugi::xml_node& node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadTileSetProperties(pugi::xml_node& node, TileSet* set);
	void AssignColliders(MapLayer* layer);

	TileSet* GetTilesetFromTileId(int id) const;
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

public:

	MapData mapData;
	List<Collider*> mapColliders;

private:

	void LogAll();
	pugi::xml_document mapFile;
	SString folder;
	SString loadingLevel;
	bool mapLoaded;
};

#endif // __MAP_H__