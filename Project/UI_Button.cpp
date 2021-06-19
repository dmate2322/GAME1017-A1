#include "UI_Button.h"
#include "Game.h"
#include "TextureManager.h"
#include "EventManager.h"

UI_Button::UI_Button(SDL_Rect source, SDL_FRect destination, const char* textureKey)
	: SpriteObject(source, destination)
	, m_textureKey(textureKey)
	, m_state(ButtonState::STATE_UP)
{

}

void UI_Button::Render()
{
	m_sourceTransform.x = m_sourceTransform.w * (int)m_state;
	SDL_RenderCopyF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture(m_textureKey), &m_sourceTransform, &m_destinationTransform);
}

void UI_Button::Update(float deltaTime)
{
	SDL_Rect intDestinationRect = { (int)m_destinationTransform.x, (int)m_destinationTransform.y, (int)m_destinationTransform.w, (int)m_destinationTransform.h };
	bool mouseOverlap = SDL_PointInRect(&EventManager::GetMousePos(), &intDestinationRect);
	switch (m_state)
	{
	case ButtonState::STATE_UP:
		if (mouseOverlap)
		{
			m_state = ButtonState::STATE_OVER;
			EventManager::SetCursor(SDL_SYSTEM_CURSOR_HAND);
		}
		break;
	case ButtonState::STATE_OVER:
		if (!mouseOverlap)
		{
			m_state = ButtonState::STATE_UP;
			EventManager::SetCursor(SDL_SYSTEM_CURSOR_ARROW);
		}
		else if (mouseOverlap && EventManager::MousePressed(1))
		{
			m_state = ButtonState::STATE_DOWN; // 1 is left mouse.
		}
		break;
	case ButtonState::STATE_DOWN:
		if (EventManager::MouseReleased(1)) // Left mouse released.
		{
			if (mouseOverlap)
			{
				m_state = ButtonState::STATE_OVER;
				Execute();
			}
			else
			{
				m_state = ButtonState::STATE_UP;
				EventManager::SetCursor(SDL_SYSTEM_CURSOR_ARROW);
			}
		}
		break;
	}
}
