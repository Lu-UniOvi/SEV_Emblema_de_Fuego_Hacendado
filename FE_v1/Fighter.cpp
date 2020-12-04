#include "Fighter.h"

Fighter::Fighter()
	: CharacterClass(new Axe(), new Infantry()) {
	className = "Fighter";
}