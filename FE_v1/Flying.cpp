#include "Flying.h"

Flying::Flying()
	: MovementType(6) {

}

float Flying::costeMovimiento(Tile* tile) {
	return tile->isWall ? numeric_limits<float>::infinity() : 1;
}

string Flying::toString() {
	return "Flying";
}
