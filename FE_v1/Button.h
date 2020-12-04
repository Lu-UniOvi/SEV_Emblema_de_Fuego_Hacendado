#pragma once

#include "Game.h"
#include "Actor.h"
#include "Text.h"

class Button {
public:
	Button(string text, int height, Game* game);
	void draw();

	Actor* button;
	Text* text;
};

