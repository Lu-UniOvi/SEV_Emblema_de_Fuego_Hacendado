#include "Infantry.h"

Infantry::Infantry()
	: MovementType(5){
	
}

float Infantry::costeMovimiento(Tile* tile) {
	return tile->movementCost;
}
