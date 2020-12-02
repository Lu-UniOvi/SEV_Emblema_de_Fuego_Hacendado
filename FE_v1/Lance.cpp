#include "Lance.h"

Lance::Lance() : WeaponType() {
	this->physicalAttack = true;
	this->closeRange = true;
	this->longRange = false;
	this->id = WeaponTypeId::LANCE;
}

float Lance::getBonus(WeaponType* wt) {
	if (wt->id == WeaponTypeId::AXE)
		return 0.8f;
	if (wt->id == WeaponTypeId::SWORD)
		return 1.2f;
	return 1.0f;
}