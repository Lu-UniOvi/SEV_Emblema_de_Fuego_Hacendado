#pragma once

#include "ButtonManagerParent.h"

#include "Game.h"
#include "GameLayer.h"

#include "Button.h"

class ButtonManager : public ButtonManagerParent {
public:
	ButtonManager(GameLayer* gl, Game* game);

	void init();
	void draw();

	bool click(float motionX, float motionY);
	void unselectButtonPaint();

	GameLayer* gl;
	Game* game;
};