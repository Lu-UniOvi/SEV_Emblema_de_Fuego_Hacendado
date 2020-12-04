#include "Axe.h"

Axe::Axe() : WeaponType() {
	this->physicalAttack = true;
	this->closeRange = true;
	this->longRange = false;
	this->id = WeaponTypeId::AXE;
}

float Axe::getBonus(WeaponType* wt) {
	if (wt->id == WeaponTypeId::SWORD)
		return 0.8f;
	if (wt->id == WeaponTypeId::LANCE)
		return 1.2f;
	return 1.0f;
}

string Axe::toString() {
	return "Axe";
}