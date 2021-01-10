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

	AddModule(render);

	logo->active = false;
	options->active = false;
	deathScene->active = false;
	winScene->active = false;

	PERF_PEEK(ptimer);
}

App::~App()
{
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

bool App::Awake()
{
	PERF_START(ptimer);

	bool ret = LoadConfig();

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		title.Create(configApp.child("title").child_value());
		win->SetTitle(title.GetString());
		organization.Create(configApp.child("organization").child_value());

		int cap = configApp.attribute("framerate_cap").as_int(-1);

		if (cap > 0) cappedMs = 1000 / cap;
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

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

void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	dt = frameTime.ReadSec();
	frameTime.Start();
}

void App::FinishUpdate()
{
	if (loadRequest == true) LoadGame();
	if (saveRequest == true) SaveGame();

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

	if (lastFrameMs < (1000 / cappedFrames))
	{
		SDL_Delay((1000 / cappedFrames) - lastFrameMs);
	}
}

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

		ret = item->data->Update(dt);
	}

	return ret;
}

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

int App::GetArgc() const
{
	return argc;
}

const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

const char* App::GetTitle() const
{
	return title.GetString();
}

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
