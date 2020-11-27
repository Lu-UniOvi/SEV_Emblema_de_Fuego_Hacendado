#pragma once

#include "Tile.h"

class MovementType {
public:
	MovementType(int movementRange);

	virtual float costeMovimiento(Tile* tile);

	int movementRange;
};