#include "DescriptionPanel.h"

DescriptionPanel::DescriptionPanel(Game* game) {
	int width = WIDTH * .83;
	int height = HEIGHT * .35;
	panel = new Actor("res/base_button.png", width, height, 150, 210, game);
	texto = new Text("a\nb", width, height, game->menuFont, 140, game);
}

void DescriptionPanel::draw() {
	panel->draw();
	texto->draw();
}