#include "Enemy.h"

Enemy::Enemy(string name, int hp, int atk, int spd, int def, int res, CharacterClass* characterClass,
	float x, float y, int width, int height, Game* game)
	: Character(name, hp, atk, spd, def, res, characterClass, x, y, width, height, game) {

}

bool Enemy::isAlly() {
	return false;
}

string Enemy::toString() {
	cout << "enemy tostring" << endl;
	string str = "";

	return str;
	//return Character::toString();
}
