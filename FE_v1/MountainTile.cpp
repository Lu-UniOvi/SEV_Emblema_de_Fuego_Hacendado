#include "MountainTile.h"

MountainTile::MountainTile(float x, float y, Game* game)
	: Tile("res/mountain.png", x, y, game) {
	movementCost = INFINITY;
	isWall = false;
}