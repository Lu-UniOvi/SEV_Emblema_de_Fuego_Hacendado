#pragma once

#include "Tile.h"

class MovementType {
public:
	MovementType(int movementRange);

	virtual int costeMovimiento(Tile* tile);

	int movementRange;
};