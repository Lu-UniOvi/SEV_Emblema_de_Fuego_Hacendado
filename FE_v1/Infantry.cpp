#include "Infantry.h"

Infantry::Infantry()
	: MovementType(5){
	
}

int Infantry::costeMovimiento(Tile* tile) {
	return tile->movementCost;
}
