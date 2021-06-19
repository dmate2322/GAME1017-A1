#pragma once
#include "SpriteObject.h"
#include <string>
#include <SDL.h>

class UI_Button : public SpriteObject
{
public:
	UI_Button(SDL_Rect source, SDL_FRect destination, const char* textureKey);
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

protected:
	std::string m_textureKey;

	enum class ButtonState
	{
		STATE_UP,
		STATE_OVER,
		STATE_DOWN
	};

	ButtonState m_state;

	virtual void Execute() = 0;
};

