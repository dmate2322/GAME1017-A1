#pragma once
#include "UI_Button.h"
class ExitButton : public UI_Button
{
public:
	ExitButton(SDL_Rect source, SDL_FRect destination, const char* textureKey);
private:
	virtual void Execute() override;
};

