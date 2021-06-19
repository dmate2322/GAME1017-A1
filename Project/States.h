#pragma once
#include <map>
#include <SDL.h>
#include "Counter.h"
#include "GameObject.h"
#include "Sprites.h"
#include <string>

class TiledLevel;

class State // This is the abstract base class for all states
{
public:
	//State() = default;
	//virtual ~State() = default; // Modern alternative to {}

	virtual void Enter() = 0; // = 0 means pure virtual - must be defined in subclass
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
	virtual void Resume() { }

	std::map<std::string, GameObject*> m_objects;
};

class TitleState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
private:
	SDL_Texture* m_pBackground;
};

class GameState : public State
{
public:
	static const int kWidth = 1024;
	static const int kHeight = 768;
	static const int kPlayerSpeed = 400;

	static const int kPlayerSourceXPosition = 0;
	static const int kPlayerSourceYPosition = 0;
	static const int kPlayerWidth = 94;
	static const int kPlayerHeight = 100;

	static const int kPlayerBulletSpeed = 600;
	static const int kPlayerBulletSourceXPosition = 376;
	static const int kPlayerBulletSourceYPosition = 0;
	static const int kPlayerBulletWidth = 10;
	static const int kPlayerBulletHeight = 100;

	static const int kEnemySourceXPosition = 0;
	static const int kEnemySourceYPosition = 100;
	static const int kEnemyWidth = 40;
	static const int kEnemyHeight = 46;

	static const int kBackgroundScrollSpeed = 30;
	static const int kNumberOfBackgrounds = 2;

	int counterFrameWidth = 8;
	int counterFrameHeight = 10;
	int counterTargetWidth = 40;
	int counterTargetHeight = 40;
	int counterDigit = 0;
	bool counterMaxDigit = false;
	
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
	virtual void Resume() override;
	void CheckCollision();

	/*void setCanShoot(bool set) { m_canShoot = set; }*/

private:
	// These three flags check if we need to clear the respective vectors of nullptrs.
	bool m_shouldClearEnemies;
	bool m_shouldClearPlayerBullets;
	bool m_shouldClearEnemyBullets;

	bool m_canShoot; // This restricts the player from firing again unless they release the Spacebar.

	int gameWon;
	
	SDL_Texture* m_pCounter;
	SDL_Texture* m_pBGText; // For the bg.
	Sprite m_bgArray[kNumberOfBackgrounds];
	
	Counter digit1;
	Counter digit2;
	int count1;
	int count2;
	
	SDL_Texture* m_pSpriteTexture; // For the sprites.
	Player* m_pPlayer;

	// The enemy spawn frame timer properties.
	float m_enemySpawnTime;
	float m_enemySpawnMaxTime;

	SDL_FPoint m_pivot;

	Mix_Music* m_pMusic;
	std::vector<Mix_Chunk*> m_sounds;

	std::vector<Enemy*> m_enemies;
	std::vector<Bullet*> m_playerBullets;
	std::vector<Bullet*> m_enemyBullets;
};

class PauseState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
};

class WinState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
private:
	SDL_Texture* m_pBackground;
};

class LoseState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
private:
	SDL_Texture* m_pBackground;
};
