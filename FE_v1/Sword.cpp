#include "Sword.h"

Sword::Sword() : WeaponType() {
	this->physicalAttack = true;
	this->closeRange = true;
	this->longRange = false;
	this->id = WeaponTypeId::SWORD;
}

float Sword::getBonus(WeaponType* wt) {
	if (wt->id == WeaponTypeId::LANCE)
		return 0.8f;
	if (wt->id == WeaponTypeId::AXE)
		return 1.2f;
	return 1.0f;
}