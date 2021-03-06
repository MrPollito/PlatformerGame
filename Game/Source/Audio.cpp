#include "App.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"


Audio::Audio() : Module()
{
	music = NULL;
	name.Create("audio");
}

Audio::~Audio()
{}

bool Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	if (config != NULL)
	{
		volume = config.child("volume").attribute("value").as_int();
	}

	return ret;
}

bool Audio::PreUpdate()
{
	return true;
}

bool Audio::Update()
{
	return true;
}

bool Audio::PostUpdate()
{
	bool ret = true;
	Mix_VolumeMusic(volume);
	return ret;
}

bool Audio::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	ListItem<Mix_Chunk*>* item;
	for (item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.Clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

bool Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if (!active)
		return false;

	if (music != NULL)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if (music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

unsigned int Audio::LoadFx(const char* path)
{
	uint ret = 0;

	if (!active)
	{
		return 0;
	}

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.Add(chunk);
		ret = fx.Count();
	}

	return ret;
}

bool Audio::PlayFx(unsigned int id, int once, int repeat)
{
	int tmp = once;
	if (tmp == 1)
	{
		if (!active)
		{
			return false;
		}

		if (id > 0 && id <= fx.Count())
		{
			Mix_PlayChannel(-1, fx[id - 1], repeat);
		}
	}
	tmp = 0;
	return true;
}

bool Audio::UnloadFx(uint index)
{
	ListItem<Mix_Chunk*>* s = fx.At(index - 1);
	if (s != nullptr)
	{
		Mix_FreeChunk(s->data);
		return fx.Del(s);
	}

	return false;
}

void Audio::Volume(int value)
{
	if (value == 1)
	{
		volume += 8;
		if (volume > 128)
		{
			volume = 128;
			LOG("Volume --> MAX");
		}
	}
	else if (value == 0)
	{
		volume -= 8;
		if (volume < 0)
		{
			volume = 0;
			LOG("Volume --> MIN");
		}
	}

	else LOG("Error increasing/decreasing volume");

}

bool Audio::Load(pugi::xml_node& node)
{
	volume = node.child("volume").attribute("value").as_int();
	return true;
}

bool Audio::Save(pugi::xml_node& node)
{
	bool ret = true;
	pugi::xml_node vol = node.append_child("volume");

	if (vol == NULL)
	{
		LOG("Error on Save function of %s", name.GetString());
		ret = false;
	}
	else
	{
		vol.append_attribute("value").set_value(volume);
	}

	return ret;
}