#include "Soldier.h"

Soldier::Soldier()
	: CharacterClass(new Lance(), new Infantry()) {
	className = "Soldier";
}