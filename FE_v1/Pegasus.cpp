#include "Pegasus.h"

Pegasus::Pegasus()
	: CharacterClass(new Lance(), new Flying()) {
	className = "Pegasus";
}