#include "Character.h"

Character::Character(string name, int hp, int atk, int spd, int def, int res, CharacterClass* characterClass,
	float x, float y, int width, int height, Game* game)
		: Actor("res/" + name + ".png", x, y, width, height, game) {
	this->name = name;

	this->characterClass = characterClass;

	this->hp = hp;
	this->atk = atk;
	this->spd = spd;
	this->def = def;
	this->res = res;

	this->currentHP = hp;
	this->canPlay = true;

	this->aIdle = new Animation("res/" + name + "_idle.png", width, height,
		120, 40, 8, 3, true, game);
	this->aRunningLeft = new Animation("res/" + name + "_run_left.png", width, height,
		160, 40, 6, 4, true, game);
	this->aRunningRight = new Animation("res/" + name + "_run_right.png", width, height,
		160, 40, 6, 4, true, game);
	this->aRunningUp = new Animation("res/" + name + "_run_up.png", width, height,
		160, 40, 6, 4, true, game);
	this->aRunningDown = new Animation("res/" + name + "_run_down.png", width, height,
		160, 40, 6, 4, true, game);
	this->animation = aIdle;
}

void Character::update() {
	animation->update();
}

void Character::draw(float scrollX) {
	if (canPlay)
		animation->draw(x - scrollX, y);
	else
		Actor::draw();
}