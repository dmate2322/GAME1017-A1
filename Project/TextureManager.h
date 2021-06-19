#pragma once
#include <map>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

class TextureManager
{
public:
	static void Load(const char* path, const std::string key);
	static void Unload(const std::string key);
	static SDL_Texture* GetTexture(const std::string key);
	static void Quit();

private:
	static std::map<std::string, SDL_Texture*> s_textures;
	TextureManager() = default;

};

