#include "Archer.h"

Archer::Archer()
	: CharacterClass(new Bow(), new Infantry()) {
	className = "Archer";
}