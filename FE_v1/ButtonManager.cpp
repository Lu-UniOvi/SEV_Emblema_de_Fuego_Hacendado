#include "ButtonManager.h"

ButtonManager::ButtonManager(GameLayer* gl, Game* game) 
	: ButtonManagerParent(nullptr, false) {
	this->gl = gl;
	this->game = game;

	this->init();
}

void ButtonManager::init() {
	this->buttonWait = new Button("Esperar", HEIGHT * .2, game);
	this->boolWait = false;
}

void ButtonManager::draw() {
	if (boolWait)
		buttonWait->draw();
}

bool ButtonManager::click(float motionX, float motionY) {
	bool buttonClicked = false;

	if (boolWait && buttonWait->button->containsPoint(motionX, motionY)) {
		gl->moveCharacter(gl->mapManager->selectedSquare);
		this->unselectButtonPaint();
		buttonClicked = true;
	}

	return buttonClicked;
}

void ButtonManager::unselectButtonPaint() {
	boolWait = false;
}