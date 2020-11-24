#include "Actor.h"

Actor::Actor(string filename, float x, float y, int width, int height, Game* game) {
	this->game = game;
	texture = game->getTexture(filename);

	this->x = x;
	this->y = y;
	// lo que mide el fichero
	this->fileWidth = width;
	this->fileHeight = height;
	// lo que va a medir en el juego
	this->width = width;
	this->height = height;
}

void Actor::draw(float scrollX) {
	SDL_Rect source;
	source.x = 0;
	source.y = 0;
	source.w = fileWidth;
	source.h = fileHeight;

	SDL_Rect destination;
	destination.x = x - width / 2 - scrollX;
	destination.y = y - height / 2;
	destination.w = width;
	destination.h = height;

	SDL_RenderCopyEx(game->renderer, texture, &source, &destination, 0, nullptr, SDL_FLIP_NONE);
}

bool Actor::isInRender(float scrollX) {
	if ((x - scrollX) - width / 2 <= WIDTH && (x - scrollX) + width / 2 >= 0 &&
		y - height / 2 <= HEIGHT && y + height / 2 >= 0) {
		return true;
	}
	return false;
}

bool Actor::containsPoint(int pointX, int pointY) {
	if (pointX <= x + width / 2 &&
		pointX >= x - width / 2 &&
		pointY <= y + height / 2 &&
		pointY >= y - height / 2)
		return true;
	return false;
}