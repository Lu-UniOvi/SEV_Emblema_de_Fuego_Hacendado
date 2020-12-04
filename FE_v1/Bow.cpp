#include "Bow.h"

Bow::Bow() : WeaponType() {
	this->physicalAttack = true;
	this->closeRange = false;
	this->longRange = true;
	this->id = WeaponTypeId::BOW;
}

float Bow::getBonus(WeaponType* wt) {
	return 1.0f;
}

string Bow::toString() {
	return "Bow";
}