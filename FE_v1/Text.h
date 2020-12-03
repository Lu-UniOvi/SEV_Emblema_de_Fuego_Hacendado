#pragma once

#include "Game.h"

class Text {
public:
	Text(string content, float x, float y, bool menuFont, int wrapLength, Game* game);
	void draw();
	string content; // texto
	int x;
	int y;
	int width;
	int height;
	Game* game; // referencia al juego
	bool menuFont;
	int wrapLength;
};