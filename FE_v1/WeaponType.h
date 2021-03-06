#pragma once

#include "Game.h"

enum class WeaponTypeId {
	SWORD, LANCE, AXE, TOME, BOW
};

class WeaponType {
public:
	WeaponType();

	bool physicalAttack;
	WeaponTypeId id;

	//rango
	bool closeRange;
	bool longRange;

	virtual float getBonus(WeaponType* wt);
	virtual string toString();
};