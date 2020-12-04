#pragma once

#include "WeaponType.h"

class Sword : public WeaponType {
public:
	Sword();

	float getBonus(WeaponType* wt) override;
	string toString() override;
};