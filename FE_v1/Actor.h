#pragma once

#include "Game.h"

class Actor {
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);

	virtual void draw(float scrollX = 0);
	bool isInRender(float scrollX = 0);
	bool containsPoint(int pointX, int pointY);

	SDL_Texture* texture;
	float x;
	float y;
	int width;
	int height;
	int fileWidth;
	int fileHeight;
	Game* game;
};