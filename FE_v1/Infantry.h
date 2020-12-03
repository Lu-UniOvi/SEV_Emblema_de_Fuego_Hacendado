#pragma once

#include "MovementType.h"
#include "Tile.h"

class Infantry : public MovementType {
public:
	Infantry();
	float costeMovimiento(Tile* tile) override;
	string toString() override;
};