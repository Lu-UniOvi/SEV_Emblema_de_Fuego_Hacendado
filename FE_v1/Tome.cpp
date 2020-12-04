#include "Tome.h"

Tome::Tome() : WeaponType() {
	this->physicalAttack = false;
	this->closeRange = true;
	this->longRange = true;
	this->id = WeaponTypeId::TOME;
}

float Tome::getBonus(WeaponType* wt) {
	return 1.0f;
}

string Tome::toString() {
	return "Tome";
}