#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"



// L03: TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString name;
	int firstGid;
	int tileWidth;
	int tileHeight;
	int spacing;
	int margin;

	SDL_Texture* texture;
	int imageWitdth;
	int imageHeight;
	int	numTilesWidth;
	int	numTilesHeight;
	int	offsetX;
	int	offsetY;

};


// L03: DONE 1: We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// L03: TODO 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	SString orientation;
	SDL_Color backgroundColor;
	MapTypes type;

	List<TileSet*> tilesets;

};

class Map : public Module
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

	SString GetLoadingLevel()const
	{
		return loadingLevel;
	}

	// L03: TODO 1: Add your struct for map info as public for now
	MapData mapData;

private: // Private functions

	bool LoadMap();
	bool LoadTileSetDetails(pugi::xml_node& node, TileSet* set);
	bool LoadTileSetImage(pugi::xml_node& node, TileSet* set);
	

private: 

	pugi::xml_document mapFile;
	SString folder;
	SString loadingLevel;
	bool mapLoaded;

};

#endif // __MAP_H__