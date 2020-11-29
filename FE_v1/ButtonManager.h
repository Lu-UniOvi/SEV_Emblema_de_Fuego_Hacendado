#pragma once

#include "ButtonManagerParent.h"

#include "Game.h"
#include "GameLayer.h"

#include "Button.h"

class ButtonManager : public ButtonManagerParent {
public:
	ButtonManager(GameLayer* gl, Game* game);

	void init() override;
	void draw() override;

	bool click(float motionX, float motionY) override;
	void unselectButtonPaint() override;

	GameLayer* gl;
	Game* game;

private:
	float multiplier;
	void initHeight();
	void incHeight();
};