#include "Monk.h"

Monk::Monk()
	: CharacterClass(new Tome(), new Infantry()) {
	className = "Monk";
}