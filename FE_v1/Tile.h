#pragma once

#include "Actor.h"
//#include "Character.h"

class Tile : public Actor {
public:
	Tile(string filename, float x, float y, Game* game);

	//void setCharacter(Character* character);

	int movementCost;
	bool isWall;

	//Character* character;
};