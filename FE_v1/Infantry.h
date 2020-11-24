#pragma once

#include "MovementType.h"
#include "Tile.h"

class Infantry : public MovementType {
public:
	Infantry();
	int costeMovimiento(Tile* tile) override;
};