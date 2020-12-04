#pragma once

#include "MovementType.h"
#include "Tile.h"

class Flying : public MovementType {
public:
	Flying();
	float costeMovimiento(Tile* tile) override;
	string toString() override;
};