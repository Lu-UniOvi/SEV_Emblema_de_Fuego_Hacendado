#pragma once

#include "WeaponType.h"

class Tome : public WeaponType {
public:
	Tome();

	float getBonus(WeaponType* wt) override;
	string toString() override;
};