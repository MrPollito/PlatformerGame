#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio();

	virtual ~Audio();

	bool Awake(pugi::xml_node&);

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	unsigned int LoadFx(const char* path);

	bool PlayFx(unsigned int fx, int once = 1, int repeat = 0);

	bool UnloadFx(uint index);

	void Volume(int value);

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&);

private:

	_Mix_Music* music;
	List<Mix_Chunk*>fx;
	int volume = 0;
};

#endif // __AUDIO_H__