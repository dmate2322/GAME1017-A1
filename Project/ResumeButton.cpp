#include "ResumeButton.h"
#include "StateManager.h"
#include "States.h"

ResumeButton::ResumeButton(SDL_Rect source, SDL_FRect destination, const char* textureKey)
	: UI_Button(source, destination, textureKey)
{

}

void ResumeButton::Execute()
{
	StateManager::ChangeState(new PauseState());
}