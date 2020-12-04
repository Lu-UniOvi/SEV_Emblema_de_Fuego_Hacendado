#pragma once

#include "Game.h"
#include "Actor.h"
#include "Button.h"
#include "Text.h"

#include "Character.h"

class ResultPanel {
public:
	ResultPanel(Game* game);

	void draw();
	void actualizaText(map<string, int> result, Character* player, Character* enemy);

	Actor* panel;
	Button* buttonAttack;
	Text* texto;
	map<string, int> result;
};

