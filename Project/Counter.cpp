#include "Counter.h"
#include <iostream>


//constructor
Counter::Counter()
{
}

//constructor

Counter::Counter(SDL_Renderer* renderer, int dstX, int dstY)
{
	pSpriteTex = IMG_LoadTexture(renderer, "Assets/Images/counter.png");

	SDL_QueryTexture(pSpriteTex, NULL, NULL, &src.w, &src.h);

	src.x = 0;
	src.y = 0;
	src.w = frameWidth;
	src.h = frameHeight;


	dst.x = dstX;
	dst.y = dstY;
	dst.w = targetWidth;
	dst.h = targetHeight;
}


//destructor
Counter::~Counter()
{
}


void Counter::cleanup()
{
	SDL_DestroyTexture(pSpriteTex);
}



void Counter::draw(SDL_Renderer* renderer, int c)
{

	src.x = c * frameCount;

	SDL_RenderCopy(renderer, pSpriteTex, &src, &dst);
}