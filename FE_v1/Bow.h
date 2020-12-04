#pragma once

#include "WeaponType.h"

class Bow : public WeaponType {
public:
	Bow();

	float getBonus(WeaponType* wt) override;
	string toString() override;
};