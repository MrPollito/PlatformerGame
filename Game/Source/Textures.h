#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "Module.h"

#include "List.h"

struct SDL_Texture;
struct SDL_Surface;

class Textures : public Module
{
public:

	Textures();

	virtual ~Textures();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool CleanUp();

	SDL_Texture* const Load(const char* path);
	SDL_Texture* const LoadSurface(SDL_Surface* surface);
	bool UnLoad(SDL_Texture* texture);
	void GetSize(const SDL_Texture* texture, uint& width, uint& height) const;

public:

	List<SDL_Texture*> textures;
};


#endif // __TEXTURES_H__