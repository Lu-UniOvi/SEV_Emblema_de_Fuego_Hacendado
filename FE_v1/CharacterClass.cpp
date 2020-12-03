#include "CharacterClass.h"

CharacterClass::CharacterClass(WeaponType* weaponType, MovementType* movementType) {
	this->weaponType = weaponType;
	this->movementType = movementType;
}

string CharacterClass::toString() {
	return className + ": Uses " + weaponType->toString() +
		"\nMovement Type: " + movementType->toString();
}