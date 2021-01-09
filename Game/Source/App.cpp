#include "App.h"

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Collisions.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Enemy.h"

#include "FadeToBlack.h"
#include "Intro.h"
#include "Menu.h"
#include "Options.h"
#include "Logo.h"
#include "DeathScene.h"
#include "WinScene.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// L07: TODO 3: Measure the amount of ms that takes to execute:
// App constructor, Awake, Start and CleanUp
// LOG the result

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	menu = new Menu();
	intro = new Intro();
	scene = new Scene();
	options = new Options();
	map = new Map();
	pathfinding = new PathFinding();
	collisions = new Collisions();
	entityManager = new EntityManager();
	fade = new FadeToBlack();
	logo = new Logo();
	deathScene = new DeathScene();
	winScene = new WinScene();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(menu);
	AddModule(intro);
	AddModule(logo);


	AddModule(pathfinding);
	AddModule(scene);
	AddModule(map);
	AddModule(options);
	AddModule(entityManager);
	AddModule(fade);
	AddModule(deathScene);
	AddModule(winScene);
	AddModule(collisions);

	// render last to swap buffer
	AddModule(render);

	logo->active = false;
	//intro->active = false;
	//menu->active = false;
	/*scene->active = false;*/
	options->active = false;
	deathScene->active = false;
	winScene->active = false;
	/*entityManager->active = false;*/

	PERF_PEEK(ptimer);
}

// Destructor
App::~App()
{
	// release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();

	configFile.reset();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(ptimer);

	// TODO 3: Load config.xml file using load_file() method from the xml_document class.
	bool ret = LoadConfig();

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// TODO 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		win->SetTitle(title.GetString());
		organization.Create(configApp.child("organization").child_value());

		// L08: TODO 1: Read from config file your framerate cap
		int cap = configApp.attribute("framerate_cap").as_int(-1);

		if (cap > 0) cappedMs = 1000 / cap;
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			// TODO 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(ptimer);

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// TODO 3: Load config from XML file
bool App::LoadConfig()
{
	bool ret = true;

	pugi::xml_parse_result result = configFile.load_file("config.xml");

	if (result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		config = configFile.child("config");
		configApp = config.child("app");
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	// L08: TODO 4: Calculate the dt: differential time since last frame
	dt = frameTime.ReadSec();
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// L02: TODO 1: This is a good place to call Load / Save methods
	if (loadRequest == true) LoadGame();
	if (saveRequest == true) SaveGame();

	// L07: TODO 4: Now calculate:
	// Amount of frames since startup
	// Amount of time since game start (use a low resolution timer)
	// Average FPS for the whole game life
	// Amount of ms took the last update
	// Amount of frames during the last second

	float averageFps = 0.0f;
	float secondsSinceStartup = 0.0f;
	uint32 lastFrameMs = 0;
	uint32 framesOnLastUpdate = 0;
	int cappedFrames = 60;

	secondsSinceStartup = (float)SDL_GetTicks() / 1000;
	averageFps = 1 / dt;
	lastFrameMs = frameTime.Read();

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f | Last Frame Ms: %02u | Last sec frames: %i | Last dt: %.3f | Time since startup: %.3f | Frame Count: %I64u | Fps cap: %i ",
		averageFps, lastFrameMs, framesOnLastUpdate, dt, secondsSinceStartup, frameCount, cappedFrames);

	app->win->SetTitle(title);

	// L08: TODO 2: Use SDL_Delay to make sure you get your capped framerate
	if (lastFrameMs < (1000 / cappedFrames))
	{
		// L08: TODO 3: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
		// Capped frames
		SDL_Delay((1000 / cappedFrames) - lastFrameMs);
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		// L08: TODO 5: Send dt as an argument to all updates, you need
		// to update module parent class and all modules that use update
		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

void App::LoadRequest(const char* filename)
{
	loadRequest = true;
	loadFileName.Create(filename);
}

void App::SaveRequest(const char* filename)
{
	saveRequest = true;
	if (saveFileName != loadFileName)
	{
		loadFileName.Create(filename);
	}
}

// L02: TODO 5: Create a method to actually load an xml file
// then call all the modules to load themselves

bool App::LoadGame()
{
	bool ret = true;
	pugi::xml_document loadDoc;

	loadDoc.load_file("savegame.xml");

	pugi::xml_node loadState = loadDoc.child("save_state");

	ListItem<Module*>* item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Load(loadState.child(item->data->name.GetString()));
		item = item->next;
	}

	LOG("Loading finished...");

	loadRequest = false;
	return ret;
}


// L02: TODO 7: Implement the xml save method for current state
bool App::SaveGame()const
{
	bool ret = true;

	pugi::xml_document newSaveFile;
	pugi::xml_node saveState = newSaveFile.append_child("save_state");

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Save(saveState.append_child(item->data->name.GetString()));
		item = item->next;
	}

	newSaveFile.save_file("savegame.xml");
	LOG("Game saved correctly");

	saveRequest = false;
	return ret;
}
