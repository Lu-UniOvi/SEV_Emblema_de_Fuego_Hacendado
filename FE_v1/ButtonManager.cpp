#include "ButtonManager.h"

ButtonManager::ButtonManager(GameLayer* gl, Game* game) 
	: ButtonManagerParent(nullptr, false) {
	this->gl = gl;
	this->game = game;

	this->init();
}

void ButtonManager::init() {
	initHeight();

	this->buttonWait = new Button("Esperar", HEIGHT * incHeight(), game);
	this->boolWait = false;
	this->buttonAttack = new Button("Ataca", HEIGHT * incHeight(), game);
	this->boolAttack = false;
}

void ButtonManager::draw() {
	initHeight();
	if (boolAttack) {
		buttonAttack->button->y = HEIGHT * multiplier;
		buttonAttack->text->y = HEIGHT * multiplier;
		buttonAttack->draw();
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
	else if (boolAttack && buttonAttack->button->containsPoint(motionX, motionY)) {
		gl->boolSeleccionaEnemigo = true;
		this->unselectButtonPaint();
		buttonClicked = true;
	}

	return buttonClicked;
}

void ButtonManager::unselectButtonPaint() {
	boolWait = false;
	boolAttack = false;
}

void ButtonManager::initHeight() {
	this->multiplier = .2;
}

float ButtonManager::incHeight() {
	this->multiplier += .1;
	return multiplier;
}
