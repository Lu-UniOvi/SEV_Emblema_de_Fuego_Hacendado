#include "DescriptionPanel.h"

DescriptionPanel::DescriptionPanel(Game* game) {
	int width = WIDTH * .83;
	int height = HEIGHT * .27;
	panel = new Actor("res/base_button.png", width, height, 150, 150, game);
	texto = new Text("a\nb", width, height, true, 140, game);
}

void DescriptionPanel::draw() {
	panel->draw();
	texto->draw();
}