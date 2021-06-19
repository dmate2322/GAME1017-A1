#pragma once
#include "UI_Button.h"
class PlayButton : public UI_Button
{
public:
	PlayButton(SDL_Rect source, SDL_FRect destination, const char* textureKey);
private:
	virtual void Execute() override;
};

