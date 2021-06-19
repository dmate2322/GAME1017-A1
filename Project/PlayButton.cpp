#include "PlayButton.h"
#include "StateManager.h"
#include "States.h"

PlayButton::PlayButton(SDL_Rect source, SDL_FRect destination, const char* textureKey)
	: UI_Button(source, destination, textureKey)
{

}

void PlayButton::Execute()
{
	StateManager::ChangeState(new GameState());
}