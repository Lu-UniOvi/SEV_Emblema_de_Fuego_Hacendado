#include "Horse.h"

Horse::Horse()
	: MovementType(6) {

}

float Horse::costeMovimiento(Tile* tile) {
	if (tile->tileId == TileId::FOREST)
		return tile->movementCost + 2;
	return tile->movementCost;
}

string Horse::toString() {
	return "Horse";
}