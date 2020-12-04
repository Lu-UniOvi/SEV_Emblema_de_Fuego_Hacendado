#include "Princess.h"

Princess::Princess()
	: CharacterClass(new Sword(), new Infantry()) {
	className = "Princess";
}