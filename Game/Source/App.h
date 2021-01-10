#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "PugiXml/src/pugixml.hpp"

class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Map;
class Collisions;
class PathFinding;
class EntityManager;

class FadeToBlack;
class Intro;
class Menu;
class Options;
class Logo;
class DeathScene;
class WinScene;

class App
{
public:

	App(int argc, char* args[]);

	virtual ~App();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void AddModule(Module* module);

	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadRequest(const char* filename);
	void SaveRequest(const char* filename);

	bool debug = false;

private:

	bool LoadConfig();

	void PrepareUpdate();

	void FinishUpdate();

	bool PreUpdate();

	bool DoUpdate();

	bool PostUpdate();

	bool LoadGame();

	bool SaveGame() const;

public:

	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	PathFinding* pathfinding;
	Map* map;
	Collisions* collisions;
	EntityManager* entityManager;
	FadeToBlack* fade;
	Intro* intro;
	Menu* menu;
	Options* options;
	Logo* logo;
	DeathScene* deathScene;
	WinScene* winScene;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool loadRequest;
	mutable bool saveRequest;
	SString loadFileName;
	mutable SString saveFileName;

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