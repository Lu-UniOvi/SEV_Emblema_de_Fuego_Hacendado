#pragma once

#include "WeaponType.h"
#include "MovementType.h"

class CharacterClass {
public:
	CharacterClass(WeaponType* weaponType, MovementType* movementType);

	virtual string toString();

	WeaponType* weaponType;
	MovementType* movementType;
	string className;
};