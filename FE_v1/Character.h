#pragma once

#include "Actor.h"
#include "Animation.h"
#include "CharacterClass.h"

class Character : public Actor {
public:
	Character(string name, int hp, int atk, int spd, int def, int res, CharacterClass* characterClass,
		float x, float y, int width, int height, Game* game);

	//Animaciones
	Animation* aIdle;
	Animation* aRunningRight;
	Animation* aRunningLeft;
	Animation* aRunningUp;
	Animation* aRunningDown;
	Animation* animation;

	//Clase
	CharacterClass* characterClass;

	//Stats
	string name;
	int hp, atk, spd, def, res;

	//Inventario

	//Variables para el juego
	bool canPlay;
	int currentHP;

	//Metodos
	void update();
	void draw(float scrollX);
};