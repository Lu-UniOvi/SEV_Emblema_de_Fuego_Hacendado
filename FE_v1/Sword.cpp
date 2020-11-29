#include "Sword.h"

Sword::Sword() : WeaponType() {
	this->physicalAttack = true;
	this->closeRange = true;
	this->longRange = false;
}