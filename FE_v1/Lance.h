#pragma once

#include "WeaponType.h"

class Lance : public WeaponType {
public:
	Lance();

	float getBonus(WeaponType* wt) override;
	string toString() override;
};