#pragma once

#include "MovementType.h"
#include "Tile.h"

class Horse : public MovementType {
public:
	Horse();
	float costeMovimiento(Tile* tile) override;
	string toString() override;
};