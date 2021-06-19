#pragma once

#include <SDL.h>

class GameObject
{
public:
	GameObject()
		: m_destinationTransform({ 0,0,0,0 })
	{ }

	GameObject(const SDL_FRect destination)
		: m_destinationTransform(destination)
	{ }

	virtual ~GameObject() = default;


	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	SDL_FRect* GetDestinationTransform() { return &m_destinationTransform; }

protected:
	SDL_FRect m_destinationTransform;
};

