#pragma once

#include "WeaponType.h"
#include "MovementType.h"

class CharacterClass {
public:
	CharacterClass(WeaponType* weaponType, MovementType* movementType);

	WeaponType* weaponType;
	MovementType* movementType;
};