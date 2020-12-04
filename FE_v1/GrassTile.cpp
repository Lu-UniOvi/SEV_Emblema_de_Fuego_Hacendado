#include "GrassTile.h"

GrassTile::GrassTile(float x, float y, Game* game)
	: Tile("res/grass.png", x, y, game) {
	movementCost = 1;
	isWall = false;
	tileId = TileId::GRASS;
}