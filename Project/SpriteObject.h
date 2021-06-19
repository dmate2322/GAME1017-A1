#pragma once

#include "GameObject.h"

class SpriteObject : public GameObject
{
public:
	SpriteObject(const SDL_Rect source, const SDL_FRect destination)
		: GameObject(destination)
		, m_sourceTransform(source)
	{ }

	SDL_Rect* GetSourceTransform() { return &m_sourceTransform; }

protected:
	SDL_Rect m_sourceTransform;
};

