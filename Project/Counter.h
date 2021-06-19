#pragma once
#include <SDL_image.h>


class Counter
{
	SDL_Texture* pSpriteTex = nullptr;
	SDL_Rect src;

public:
	SDL_Rect dst;

	int displayFrame = 0;

	//How mnay frames there are
	int frameCount = 8;


	/*
	Total size of the image: 768x32
	Each frame size: 32x32
		Width: Total Width / Frame Count; 768/24 = 32 pixels
		Height: 32 pixels because there's only a single row
	*/
	int frameWidth = 8;
	int frameHeight = 10;
	int targetWidth = 40;
	int targetHeight = 40;
	int digit = 0;
	bool maxDigit = false;
	// constructors -- these are used when initializing an object
	Counter();
	Counter(SDL_Renderer* renderer, int dstX, int dstY);


	// destructor -- used when the object is to be removed from memory
	~Counter();


	void draw(SDL_Renderer* renderer, int c);
	void cleanup();
};