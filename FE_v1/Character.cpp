#include "Character.h"

Character::Character(string name, int hp, int atk, int spd, int def, int res, CharacterClass* characterClass,
	float x, float y, int width, int height, Game* game)
		: Actor("res/" + name + ".png", x, y, width, height, game) {
	this->name = name;

	this->characterClass = characterClass;

	this->hp = hp;
	this->atk = atk;
	this->spd = spd;
	this->def = def;
	this->res = res;

	this->currentHP = hp;
	this->canPlay = true;

	this->aIdle = new Animation("res/" + name + "_idle.png", width, height,
		120, 40, 8, 3, true, game);
	/*this->aRunningLeft = new Animation("res/" + name + "_run_left.png", width, height,
		160, 40, 6, 4, true, game);
	this->aRunningRight = new Animation("res/" + name + "_run_right.png", width, height,
		160, 40, 6, 4, true, game);
	this->aRunningUp = new Animation("res/" + name + "_run_up.png", width, height,
		160, 40, 6, 4, true, game);
	this->aRunningDown = new Animation("res/" + name + "_run_down.png", width, height,
		160, 40, 6, 4, true, game);*/
	this->animation = aIdle;
}

void Character::update() {
	animation->update();
}

void Character::draw(float scrollX) {
	if (canPlay)
		animation->draw(x - scrollX, y);
	else
		Actor::draw();
}

bool Character::isAlly() {
	return true;
}

map<string, int> Character::checkAttack(Character* target) {
	int damageDealt = 0;
	int damageTaken = 0;

	map<string, int> result;
	result["damageDealt"] = damageDealt;
	result["damageTaken"] = damageTaken;

	//this ataca primero
	damageDealt = this->calculateDamage(target);
	result["damageDealt"] = damageDealt;

	//Comprueba si el target se murio
	if (target->currentHP <= damageDealt) {
		return result;
	}

	//target ataca si puede
	if (true) { //se comprobaría el rango del ataque
		damageTaken = target->calculateDamage(this);
		result["damageTaken"] = damageTaken;
	}

	//Comprueba si this se murio
	if (this->currentHP <= damageTaken) {
		return result;
	}

	//Se comprueba si this hace doble
	if (this->spd - target->spd >= 5) {
		//hace doble y vuelve a atacar
		damageDealt += this->calculateDamage(target);
		result["damageDealt"] = damageDealt;
	}

	//Comprueba si el target se murio
	if (target->currentHP <= damageDealt) {
		return result;
	}

	//Se comprueba si target hace doble
	if (target->spd - this->spd >= 5) {
		//hace doble y vuelve a atacar
		damageTaken += target->calculateDamage(this);
		result["damageTaken"] = damageTaken;
	}

	//Comprueba si el target se murio
	if (this->currentHP <= damageTaken) {
		return result;
	}

	return result;
}

int Character::calculateDamage(Character* target) {
	if (this->characterClass->weaponType->physicalAttack) {
		//Ataca por defensa
		return this->atk * characterClass->weaponType->
			getBonus(target->characterClass->weaponType) - target->def; //Le debería sumar el ataque del arma
	}
	else {
		//Ataca por resistencia
		return this->atk * characterClass->weaponType->
			getBonus(target->characterClass->weaponType) - target->res; //Le debería sumar el ataque del arma
	}
}

string Character::toString() {
	string str = string();
	
	str += "HP: " + to_string(currentHP) + " / " + to_string(hp);
	str += "\n";
	str += "Atk: " + to_string(atk);
	str += "\n";
	str += "Spd: " + to_string(spd);
	str += "\n";
	str += "Def: " + to_string(def);
	str += "\n";
	str += "Res: " + to_string(res);
	str += "\n";

	return str;
}
