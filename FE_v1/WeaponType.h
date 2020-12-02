#pragma once

enum class WeaponTypeId {
	SWORD, LANCE, AXE, TOME
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
};