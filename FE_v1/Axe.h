#pragma once

#include "WeaponType.h"

class Axe : public WeaponType {
public:
	Axe();

	float getBonus(WeaponType* wt) override;
	string toString() override;
};