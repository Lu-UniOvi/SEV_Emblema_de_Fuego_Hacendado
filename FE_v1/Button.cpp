#include "Button.h"

Button::Button(string text, int height, Game* game) {
	this->button = new Actor("res/base_button.png", WIDTH * .13, height, 125, 30, game);
	this->text = new Text(text, WIDTH * .13, height, true, 350, game);
}

void Button::draw() {
	button->draw();
	text->draw();
}