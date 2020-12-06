#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "PugiXml/src/pugixml.hpp"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Map;
class Collisions;
class Player;
class PathFinding;
class PigEnemy;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// L02: TODO 1: Create methods to request Load / Save
	void LoadRequest(const char* filename);
	void SaveRequest(const char* filename);

	bool debug = false;

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// L02: TODO 5: Declare methods to load/save game
	bool LoadGame();
	bool SaveGame()const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	PathFinding* pathfinding;
	Map* map;
	Collisions* collisions;
	Player* player;
	PigEnemy* pigEnemy;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	// L01: TODO 2: Create two new variables from pugui namespace:
	// a xml_document to store the config file and
	// two xml_node to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	// L02: TODO 1: Create required variables to request load / save and 
	// the filename for save / load
	bool loadRequest;
	mutable bool saveRequest;
	SString loadFileName;
	mutable SString saveFileName;

	// L07: TODO 4: Calculate some timing measures
	// required variables are provided:
	PerfTimer ptimer;
	uint64 frameCount = 0;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	float dt = 0.0f;

	float cappedMs = -1;
};

extern App* app;

#endif	// __APP_H__