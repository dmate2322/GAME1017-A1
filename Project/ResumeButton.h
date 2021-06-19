#pragma once
#include "UI_Button.h"
class ResumeButton : public UI_Button
{
public:
	ResumeButton(SDL_Rect source, SDL_FRect destination, const char* textureKey);
private:
	virtual void Execute() override;
};
