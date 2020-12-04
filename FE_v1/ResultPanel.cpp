#include "ResultPanel.h"

ResultPanel::ResultPanel(Game* game) {
	panel = new Actor("res/base_button.png", WIDTH * .5, HEIGHT * .5, 400, 200, game);
	buttonAttack = new Button("Ataca", HEIGHT * .90, game);
	buttonAttack->button->x = WIDTH * .5;
	buttonAttack->text->x = WIDTH * .5;
	texto = new Text("a\nb", WIDTH * .5, HEIGHT * .5, true, 350, game);
}

void ResultPanel::draw() {
	panel->draw();
	buttonAttack->draw();
	texto->draw();
}

void ResultPanel::actualizaText(map<string, int> result, Character* player, Character* enemy) {
	this->result = result;
	string str;
	int aux;

	str += "Damage dealt from player: ";
	str += to_string(result["damageDealt"]);
	str += "\nDamage dealt from enemy: ";
	str += to_string(result["damageTaken"]);
	str += "\nPlayer's HP after combat: ";
	aux = player->currentHP > result["damageTaken"] ?
		player->currentHP - result["damageTaken"] : 0;
	str += to_string(aux);
	str += "\nEnemy's HP after combat: ";
	aux = enemy->currentHP > result["damageDealt"] ?
		enemy->currentHP - result["damageDealt"] : 0;
	str += to_string(aux);

	texto->content = str;
}
