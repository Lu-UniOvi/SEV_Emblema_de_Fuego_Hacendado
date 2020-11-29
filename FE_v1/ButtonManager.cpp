#include "ButtonManager.h"

ButtonManager::ButtonManager(GameLayer* gl, Game* game) 
	: ButtonManagerParent(nullptr, false) {
	this->gl = gl;
	this->game = game;

	this->init();
}

void ButtonManager::init() {
	initHeight();

	this->buttonWait = new Button("Esperar", HEIGHT * this->multiplier, game);
	this->boolWait = false;
	incHeight();
	this->buttonWait2 = new Button("Esperar", HEIGHT * this->multiplier, game);
	this->boolWait2 = false;
	incHeight();
}

void ButtonManager::draw() {
	initHeight();
	if (boolWait2) {
		buttonWait2->button->y = HEIGHT * multiplier;
		buttonWait2->text->y = HEIGHT * multiplier;
		buttonWait2->draw();
		incHeight();
	}
	if (boolWait) {
		buttonWait->button->y = HEIGHT * multiplier;
		buttonWait->text->y = HEIGHT * multiplier;
		buttonWait->draw();
		incHeight();
	}
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
	boolWait2 = false;
}

void ButtonManager::initHeight() {
	this->multiplier = .2;
}

void ButtonManager::incHeight() {
	this->multiplier += .1;
}
