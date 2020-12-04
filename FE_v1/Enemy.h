#pragma once

#include "Character.h"

class Enemy : public Character {
public:
	Enemy(string name, int hp, int atk, int spd, int def, int res, CharacterClass* characterClass,
		float x, float y, int width, int height, Game* game);

	//Metodos
	bool isAlly() override;
	void draw(float scrollX) override;
};