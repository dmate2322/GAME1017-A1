#include <iostream>

#include "States.h"
#include "Game.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "ExitButton.h"
#include "StateManager.h"
#include "Utilities.h"
#include "PlayButton.h"
#include "ResumeButton.h"


// Begin TitleState
void TitleState::Enter()
{
	std::cout << "Entering TitleState..." << std::endl;
	m_pBackground = IMG_LoadTexture(Game::GetInstance().GetRenderer(), "Assets/Images/titleState.png");

	TextureManager::Load("Assets/Images/Buttons/play.png", "play");

	int buttonWidth = 400;
	int buttonHeight = 100;
	float buttonX = Game::GetInstance().kWidth / 2 - buttonWidth / 2.0f;
	float buttonY = Game::GetInstance().kHeight / 2 - buttonHeight / 400.0f;
	m_objects.emplace("play", new PlayButton({ 0, 0, buttonWidth, buttonHeight }, { buttonX, buttonY, (float)buttonWidth, (float)buttonHeight }, "play"));
}

void TitleState::Update(float deltaTime)
{
	//if (Game::GetInstance().KeyDown(SDL_SCANCODE_N))
	//{
	//	std::cout << "Changing to GameState" << std::endl;
	//	StateManager::ChangeState(new GameState());// Change to new GameState
	//}
	for (auto const& i : m_objects)
	{
		i.second->Update(deltaTime);
		if (StateManager::StateChanging())
			return;
	}
}

void TitleState::Render()
{
	std::cout << "Rendering TitleState..." << std::endl;
	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 255, 255);
	SDL_RenderClear(Game::GetInstance().GetRenderer());
	SDL_RenderCopyF(Game::GetInstance().GetRenderer(), m_pBackground, 0, 0);
	for (auto const& i : m_objects)
	{
		i.second->Render();
	}
}

void TitleState::Exit()
{
	std::cout << "Exiting TitleState..." << std::endl;
	TextureManager::Unload("play");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
}
// End TitleState
void GameState::CheckCollision()
{
	// Player vs. Enemy.
	SDL_Rect playerCollisionRect = { (int)m_pPlayer->GetDestinationTransform()->x - kPlayerHeight, (int)m_pPlayer->GetDestinationTransform()->y, kPlayerHeight, kPlayerWidth };
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		SDL_Rect enemyCollisionRect = { (int)m_enemies[i]->GetDestinationTransform()->x, (int)m_enemies[i]->GetDestinationTransform()->y - kEnemyWidth, kEnemyHeight, kEnemyWidth };
		if (SDL_HasIntersection(&playerCollisionRect, &enemyCollisionRect))
		{
			// Game over!

			std::cout << "Player goes boom!" << std::endl;
			Mix_PlayChannel(-1, m_sounds[2], 0);
			delete m_enemies[i];
			m_enemies[i] = nullptr;
			m_shouldClearEnemies = true;
			StateManager::PushState(new LoseState()); // Change to new LoseState


		}
	}

	if (m_shouldClearEnemies)
	{
		CleanVector<Enemy*>(m_enemies, m_shouldClearEnemies);
	}

	// Player bullets vs. Enemies.
	for (size_t i = 0; i < m_playerBullets.size(); i++)
	{
		SDL_Rect bulletCollisionRect = { (int)m_playerBullets[i]->GetDestinationTransform()->x - kPlayerBulletHeight, (int)m_playerBullets[i]->GetDestinationTransform()->y, kPlayerBulletHeight, kPlayerBulletWidth };
		for (size_t j = 0; j < m_enemies.size(); j++)
		{
			if (m_enemies[j] != nullptr)
			{
				SDL_Rect enemyCollisionRect = { (int)m_enemies[j]->GetDestinationTransform()->x, (int)m_enemies[j]->GetDestinationTransform()->y - kEnemyWidth, kEnemyHeight, kEnemyWidth };
				if (SDL_HasIntersection(&bulletCollisionRect, &enemyCollisionRect))
				{
					count2++;
					if (count2 > 9)
					{
						count1++;
						count2 = 0;

					}
					Mix_PlayChannel(-1, m_sounds[2], 0);
					delete m_enemies[j];
					m_enemies[j] = nullptr;
					delete m_playerBullets[i];
					m_playerBullets[i] = nullptr;
					m_shouldClearEnemies = true;
					m_shouldClearPlayerBullets = true;
					break;
				}
			}
		}
	}

	if (m_shouldClearEnemies)
	{
		CleanVector<Enemy*>(m_enemies, m_shouldClearEnemies);
	}

	if (m_shouldClearPlayerBullets)
	{
		CleanVector<Bullet*>(m_playerBullets, m_shouldClearPlayerBullets);
	}

	// Enemy bullets vs. player.
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		SDL_Rect enemyBulletCollisionRect = { (int)m_enemyBullets[i]->GetDestinationTransform()->x, (int)m_enemyBullets[i]->GetDestinationTransform()->y, (int)m_enemyBullets[i]->GetDestinationTransform()->w, (int)m_enemyBullets[i]->GetDestinationTransform()->h };
		if (SDL_HasIntersection(&playerCollisionRect, &enemyBulletCollisionRect))
		{
			// Game over!
			std::cout << "Player goes boom!" << std::endl;
			Mix_PlayChannel(-1, m_sounds[2], 0);
			delete m_enemyBullets[i];
			m_enemyBullets[i] = nullptr;
			m_shouldClearEnemyBullets = true;
			StateManager::ChangeState(new LoseState()); // Change to new LoseState
			break;
		}
	}

	if (m_shouldClearEnemyBullets)
	{
		CleanVector<Bullet*>(m_enemyBullets, m_shouldClearEnemyBullets);
	}
}
// Begin GameState
void GameState::Enter() // Used for initialization.
{
	std::cout << "Entering GameState..." << std::endl;
	m_shouldClearEnemies=(false);
	m_shouldClearPlayerBullets=(false);
	m_shouldClearEnemyBullets=(false);
	m_canShoot=(true);
	m_pBGText=(nullptr);
	m_pSpriteTexture=(nullptr);
	m_pPlayer=(nullptr);
	m_enemySpawnTime=(0);
	m_enemySpawnMaxTime=(1.5f);
	m_pivot={ 0,0 };
	m_pMusic=(nullptr);
	count1 = 0;
	count2 = 0;
	gameWon = 2;
	m_pBGText = IMG_LoadTexture(Game::GetInstance().GetRenderer(), "Assets/Images/background.png");
	m_pSpriteTexture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), "Assets/Images/sprites.png");
	digit1 = Counter(Game::GetInstance().GetRenderer(), 0, 0);
	digit2 = Counter(Game::GetInstance().GetRenderer(), 45, 0);
	
	m_bgArray[0] = { {0, 0, kWidth, kHeight}, {0, 0, kWidth, kHeight} };
	m_bgArray[1] = { {0, 0, kWidth, kHeight}, {kWidth, 0, kWidth, kHeight} };
	
	m_pMusic = Mix_LoadMUS("Assets/Audio/game.mp3"); // Load the music track.
	if (m_pMusic == nullptr)
	{
		std::cout << "Failed to load music: " << Mix_GetError() << std::endl;
	}

	m_sounds.reserve(3);
	Mix_Chunk* sound = Mix_LoadWAV("Assets/Audio/enemy.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	sound = Mix_LoadWAV("Assets/Audio/laser.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	sound = Mix_LoadWAV("Assets/Audio/explode.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	m_pPlayer = new Player({ kPlayerSourceXPosition, kPlayerSourceYPosition, kPlayerWidth, kPlayerHeight }, // Image Source
		{ kWidth / 4 ,kHeight / 2 - kPlayerHeight / 2,kPlayerWidth,kPlayerHeight }); // Window Destination

	Mix_PlayMusic(m_pMusic, -1); // Play. -1 = looping.
	Mix_VolumeMusic(0); // 0-MIX_MAX_VOLUME (128).


}

void GameState::Update(float deltaTime)
{
	
	if (EventManager::KeyPressed(SDL_SCANCODE_X))
	{
		StateManager::ChangeState(new TitleState()); // Change to new TitleState
	}
	else if (EventManager::KeyPressed(SDL_SCANCODE_P))
	{
		StateManager::PushState(new PauseState()); // Add new PauseState
	}
	else
	{
	

		
	}

	// Scroll the backgrounds. 
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		m_bgArray[i].GetDestinationTransform()->x -= kBackgroundScrollSpeed * deltaTime;
	}

	// Check if they need to snap back.
	if (m_bgArray[1].GetDestinationTransform()->x <= 0)
	{
		m_bgArray[0].GetDestinationTransform()->x = 0;
		m_bgArray[1].GetDestinationTransform()->x = Game::GetInstance().getWidth();
	}

	// Player animation/movement.
	m_pPlayer->Animate(deltaTime); // Oh! We're telling the player to animate itself. This is good! Hint hint.

	if (EventManager::KeyHeld(SDL_SCANCODE_A) && m_pPlayer->GetDestinationTransform()->x > m_pPlayer->GetDestinationTransform()->h)
	{
		m_pPlayer->GetDestinationTransform()->x -= kPlayerSpeed * deltaTime;
	}
	else if (EventManager::KeyHeld(SDL_SCANCODE_D) && m_pPlayer->GetDestinationTransform()->x < kWidth / 2)
	{
		m_pPlayer->GetDestinationTransform()->x += kPlayerSpeed * deltaTime;
	}
	if (EventManager::KeyHeld(SDL_SCANCODE_W) && m_pPlayer->GetDestinationTransform()->y > 0)
	{
		m_pPlayer->GetDestinationTransform()->y -= kPlayerSpeed * deltaTime;
	}
	else if (EventManager::KeyHeld(SDL_SCANCODE_S) && m_pPlayer->GetDestinationTransform()->y < kHeight - m_pPlayer->GetDestinationTransform()->w)
	{
		m_pPlayer->GetDestinationTransform()->y += kPlayerSpeed * deltaTime;
	}

	if (EventManager::KeyHeld(SDL_SCANCODE_SPACE) && Game::GetInstance().getCanShoot())
	{
		Game::GetInstance().setCanShoot(false);
		m_playerBullets.push_back(
			new Bullet({ kPlayerBulletSourceXPosition, kPlayerBulletSourceYPosition, kPlayerBulletWidth, kPlayerBulletHeight },
				{ m_pPlayer->GetDestinationTransform()->x + kPlayerWidth - kPlayerBulletWidth, m_pPlayer->GetDestinationTransform()->y + kPlayerHeight / 2 - kPlayerBulletWidth, kPlayerBulletWidth, kPlayerBulletHeight },
				kPlayerBulletSpeed));
		Mix_PlayChannel(-1, m_sounds[1], 0);
	}

	// Enemy animation/movement.
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->Update(deltaTime);
		if (m_enemies[i]->GetDestinationTransform()->x < -m_enemies[i]->GetDestinationTransform()->h)
		{
			delete m_enemies[i];
			m_enemies[i] = nullptr;
			m_shouldClearEnemies = true;
		}
	}
	if (m_shouldClearEnemies)
	{
		CleanVector<Enemy*>(m_enemies, m_shouldClearEnemies);
	}

	// Update enemy spawns.
	
	m_enemySpawnTime += deltaTime;
	
	if (m_enemySpawnTime > m_enemySpawnMaxTime)
	{
		// Randomizing enemy bullet spawn rate
		float bulletSpawnRate = 0.5f + (rand() % 3) / 2.0f;
		// Random starting y location
		float yEnemyLocation = (float)(kEnemyHeight + rand() % (kHeight - kEnemyHeight));
		m_enemies.push_back(
			new Enemy({ kEnemySourceXPosition, kEnemySourceYPosition, kEnemyWidth, kEnemyHeight },
				{ kWidth, yEnemyLocation, kEnemyWidth, kEnemyHeight },
				&m_enemyBullets,
				m_sounds[0],
				bulletSpawnRate));

		m_enemySpawnTime = 0;
	}

	// Update the bullets. Player's first.
	for (size_t i = 0; i < m_playerBullets.size(); i++)
	{
		m_playerBullets[i]->Update(deltaTime);
		if (m_playerBullets[i]->GetDestinationTransform()->x > kWidth)
		{
			delete m_playerBullets[i];
			m_playerBullets[i] = nullptr;
			m_shouldClearPlayerBullets = true;
		}
	}

	if (m_shouldClearPlayerBullets)
	{
		CleanVector<Bullet*>(m_playerBullets, m_shouldClearPlayerBullets);
	}

	// Now enemy bullets.
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		m_enemyBullets[i]->Update(deltaTime);
		if (m_enemyBullets[i]->GetDestinationTransform()->x < -m_enemyBullets[i]->GetDestinationTransform()->w)
		{
			delete m_enemyBullets[i];
			m_enemyBullets[i] = nullptr;
			m_shouldClearEnemyBullets = true;
		}
	}

	if (m_shouldClearEnemyBullets)
	{
		CleanVector<Bullet*>(m_enemyBullets, m_shouldClearEnemyBullets);
	}

	CheckCollision();

	//Check for win
	if (count2 == gameWon)
	{
		StateManager::ChangeState(new WinState()); // Change to new WinState
	}

	
	
}

void GameState::Render()
{
	auto m_pRenderer = Game::GetInstance().GetRenderer();
	std::cout << "Rendering GameState..." << std::endl;
	SDL_Renderer* pRenderer = Game::GetInstance().GetRenderer();

	SDL_SetRenderDrawColor(pRenderer, 0, 0, 255, 255);
	SDL_RenderClear(pRenderer);

	// Render stuff. Background first.
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		SDL_RenderCopyF(m_pRenderer, m_pBGText, m_bgArray[i].GetSourceTransform(), m_bgArray[i].GetDestinationTransform());
	}

	// Player.
	SDL_RenderCopyExF(m_pRenderer,
		m_pSpriteTexture,
		m_pPlayer->GetSourceTransform(),
		m_pPlayer->GetDestinationTransform(),
		m_pPlayer->GetAngle(),
		&m_pivot,
		SDL_FLIP_NONE);

	// Player bullets.	
	for (size_t i = 0; i < m_playerBullets.size(); i++)
	{
		SDL_RenderCopyExF(m_pRenderer, m_pSpriteTexture, m_playerBullets[i]->GetSourceTransform(), m_playerBullets[i]->GetDestinationTransform(), 90, &m_pivot, SDL_FLIP_NONE);
	}

	// Enemies.
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		SDL_RenderCopyExF(m_pRenderer, m_pSpriteTexture, m_enemies[i]->GetSourceTransform(), m_enemies[i]->GetDestinationTransform(), -90, &m_pivot, SDL_FLIP_NONE);
	}

	// Enemy bullets.
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		SDL_RenderCopyF(m_pRenderer, m_pSpriteTexture, m_enemyBullets[i]->GetSourceTransform(), m_enemyBullets[i]->GetDestinationTransform());
	}

	//For Counter
	digit1.draw(m_pRenderer, count1);
	digit2.draw(m_pRenderer, count2);
	

}



void GameState::Exit()
{
	std::cout << "Exiting GameState..." << std::endl;

	// Delete Player
	delete m_pPlayer;
	m_pPlayer = nullptr;

	// Destroy Enemies
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		delete m_enemies[i];
		m_enemies[i] = nullptr;
	}
	m_enemies.clear();

	// Destroy Player Bullets
	for (size_t i = 0; i < m_playerBullets.size(); i++)
	{
		delete m_playerBullets[i];
		m_playerBullets[i] = nullptr;
	}
	m_playerBullets.clear();

	// Destroy Enemy Bullets
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		delete m_enemyBullets[i];
		m_enemyBullets[i] = nullptr;
	}
	m_enemyBullets.clear();
	
	// Clean sounds up
	for (size_t i = 0; i < m_sounds.size(); i++)
	{
		Mix_FreeChunk(m_sounds[i]);
	}
	m_sounds.clear();

	Mix_FreeMusic(m_pMusic);
	Mix_CloseAudio();
	Mix_Quit();
	
}

void GameState::Resume()
{
	std::cout << "Resuming GameState..." << std::endl;
}
// End GameState

// Begin PauseState
void PauseState::Enter()
{
	std::cout << "Entering PauseState..." << std::endl;

	TextureManager::Load("Assets/Images/Buttons/resume.png", "resume");

	int buttonWidth = 200;
	int buttonHeight = 80;
	float buttonX = Game::GetInstance().kWidth / 2 - buttonWidth / 2.0f;
	float buttonY = Game::GetInstance().kHeight / 2 - buttonHeight / 2.0f;
	m_objects.emplace("resume", new ResumeButton({ 0, 0, buttonWidth, buttonHeight }, { buttonX, buttonY, (float)buttonWidth, (float)buttonHeight }, "resume"));
}

void PauseState::Update(float deltaTime)
{
	/*if (Game::GetInstance().KeyDown(SDL_SCANCODE_R))
	{
		StateManager::PopState();
	}*/
	for (auto const& i : m_objects)
	{
		i.second->Update(deltaTime);
		if (StateManager::StateChanging())
		{
			StateManager::PopState();
			
		}
		return;
	}
}

void PauseState::Render()
{
	std::cout << "Rendering PauseState..." << std::endl;
	// First render the GameState
	StateManager::GetStates().front()->Render();
	// Now render rest of PauseState
	SDL_SetRenderDrawBlendMode(Game::GetInstance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 128, 128, 128, 128);
	SDL_Rect rect = { 256, 128, 512, 512 };
	SDL_RenderFillRect(Game::GetInstance().GetRenderer(), &rect);
	for (auto const& i : m_objects)
	{
		i.second->Render();
	}
}

void PauseState::Exit()
{
	TextureManager::Unload("resume");

	std::cout << "Exiting PauseState..." << std::endl;
}
// End PauseState

// Begin WinState
void WinState::Enter()
{
	std::cout << "Entering WinState..." << std::endl;
	m_pBackground = IMG_LoadTexture(Game::GetInstance().GetRenderer(), "Assets/Images/winState.png");
	TextureManager::Load("Assets/Images/Buttons/exit.png", "exit");

	int buttonWidth = 400;
	int buttonHeight = 100;
	float buttonX = Game::GetInstance().kWidth / 2 - buttonWidth / 2.0f;
	float buttonY = Game::GetInstance().kHeight / 1.7 - buttonHeight / 2.0f;
	m_objects.emplace("exit", new ExitButton({ 0, 0, buttonWidth, buttonHeight }, { buttonX, buttonY, (float)buttonWidth, (float)buttonHeight }, "exit"));
}

void WinState::Update(float deltaTime)
{
	for (auto const& i : m_objects)
	{
		i.second->Update(deltaTime);
		if (StateManager::StateChanging())
			return;
	}
}

void WinState::Render()
{
	std::cout << "Rendering WinState..." << std::endl;
	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 0, 255, 0, 255);
	SDL_RenderClear(Game::GetInstance().GetRenderer());
	SDL_RenderCopyF(Game::GetInstance().GetRenderer(), m_pBackground, 0, 0);
	for (auto const& i : m_objects)
	{
		i.second->Render();
	}
	
}

void WinState::Exit()
{
	TextureManager::Unload("exit");
	std::cout << "Exiting WinState..." << std::endl;
}
// End WinState

// Begin LoseState
void LoseState::Enter()
{
	std::cout << "Entering LoseState..." << std::endl;
	m_pBackground = IMG_LoadTexture(Game::GetInstance().GetRenderer(), "Assets/Images/loseState.png");

	TextureManager::Load("Assets/Images/Buttons/exit.png", "exit");

	int buttonWidth = 400;
	int buttonHeight = 100;
	float buttonX = Game::GetInstance().kWidth / 2.2f - buttonWidth / 2.0f;
	float buttonY = Game::GetInstance().kHeight / 1.7f - buttonHeight / 2.0f;
	m_objects.emplace("exit", new ExitButton({ 0, 0, buttonWidth, buttonHeight }, { buttonX, buttonY, (float)buttonWidth, (float)buttonHeight }, "exit"));
}


void LoseState::Update(float deltaTime)
{
	for (auto const& i : m_objects)
	{
		i.second->Update(deltaTime);
		if (StateManager::StateChanging())
			return;
	}
}

void LoseState::Render()
{
	std::cout << "Rendering LoseState..." << std::endl;
	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, 255);
	SDL_RenderClear(Game::GetInstance().GetRenderer());
	SDL_RenderCopyF(Game::GetInstance().GetRenderer(), m_pBackground, 0, 0);
	for (auto const& i : m_objects)
	{
		i.second->Render();
	}
}

void LoseState::Exit()
{
	TextureManager::Unload("exit");
	std::cout << "Exiting LoseState..." << std::endl;
}
// End LoseState

