#include "Text.h"

Text::Text(string content, float x, float y, bool menuFont, int wrapLength, Game* game) {
	this->content = content;
	this->x = x;
	this->y = y;
	this->game = game;
	this->menuFont = menuFont;
	this->wrapLength = wrapLength;
}

void Text::draw() {
	SDL_Color color;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 255; //transparente

	TTF_Font* font = menuFont ? game->menuFont : game->font;

	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, content.c_str(), color, wrapLength);
	// c_str() transforma el string a cost *char;
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(game->renderer, surface);

	SDL_Rect rect; // Base de coordenadas esquina superior izquierda
	rect.x = x - surface->w / 2;
	rect.y = y - surface->h / 2;
	rect.w = surface->w;
	rect.h = surface->h;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(game->renderer, Texture, NULL, &rect);
	SDL_DestroyTexture(Texture);
}