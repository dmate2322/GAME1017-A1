#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include "Sprites.h"
#include <chrono>

class Game
{
public:
	static const int kWidth = 1024;
	static const int kHeight = 768;
	
private:
	
	const Uint8* m_keyStates;
	bool m_pCanShoot;
public:
	
	static Game& GetInstance();
	int Init(const char* title, int xPos, int yPos);
	bool IsRunning();
	void HandleEvents();
	bool KeyDown(SDL_Scancode key);
	void Update(float deltaTime);
	void Render();
	void Clean();
	void QuitGame() { m_running = false; }
	
	void setCanShoot(bool set) { m_pCanShoot = set; }
	bool getCanShoot() { return m_pCanShoot; }
	
	static const int getHeight() { return kHeight; }
	static const int getWidth() { return kWidth; }

	SDL_Window* GetWindow() { return m_pWindow; }
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	const Uint8* getKeyStates() { return m_keyStates; }
	float getDeltaTime() { return m_deltaTime; }
private:
	Game();
	bool m_running; 
	SDL_Window* m_pWindow; 
	SDL_Renderer* m_pRenderer; 



	//For framerate
	const int TARGET_FRAMERATE = 60;
	int frameDelayMs = 1000 / TARGET_FRAMERATE;

	unsigned int currentFrameEndTime;
	unsigned int currentFrameStartTime;
	unsigned int timeSinceLastFrame;

	std::chrono::steady_clock::time_point m_lastFrameTime;
	float m_deltaTime;

};
