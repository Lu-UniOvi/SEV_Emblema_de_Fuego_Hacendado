#pragma once

#include "Actor.h"
//#include "Character.h"

enum class TileId {
	GRASS, FOREST, MOUNTAIN
};

class Tile : public Actor {
public:
	Tile(string filename, float x, float y, Game* game);

	//void setCharacter(Character* character);

	float movementCost;
	bool isWall;
	TileId tileId;

	//Character* character;
};