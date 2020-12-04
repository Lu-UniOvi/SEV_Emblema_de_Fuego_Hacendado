#include "ForestTile.h"

ForestTile::ForestTile(float x, float y, Game* game)
	: Tile("res/forest.png", x, y, game) {
	movementCost = 3;
	isWall = false;
	tileId = TileId::FOREST;
}