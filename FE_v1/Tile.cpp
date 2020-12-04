#include "Tile.h"
//#include "Character.h"

Tile::Tile(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 40, 40, game) {
	//this->character = nullptr;
}

//void Tile::setCharacter(Character* character) {
//	this->character = character;
//}