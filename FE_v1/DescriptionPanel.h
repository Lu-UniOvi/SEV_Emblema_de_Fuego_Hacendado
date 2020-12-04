#pragma once

#include "Game.h"
#include "Actor.h"
#include "Text.h"

class DescriptionPanel {
public:
	DescriptionPanel(Game* game);

	void draw();

	Actor* panel;
	Text* texto;
};