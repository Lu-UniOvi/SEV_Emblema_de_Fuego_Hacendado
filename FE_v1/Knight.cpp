#include "Knight.h"

Knight::Knight()
	: CharacterClass(new Lance(), new Horse()) {
	className = "Knight";
}