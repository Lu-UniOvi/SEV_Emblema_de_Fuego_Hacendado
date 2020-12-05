#include "MenuLayer.h"

MenuLayer::MenuLayer(Game* game)
	: Layer(game) {
	init();
}

void MenuLayer::init() {
	background = new Background("res/Pantalla inicio.png", WIDTH * .5,
		HEIGHT * .5, game);
	button = new Button("Iniciar", HEIGHT * 0.75, game);
	button->button->x = WIDTH * 0.5;
	button->text->x = WIDTH * 0.5;
	text = new Text("Emblema de Fuego\nVersión Hacendado", WIDTH * .5, HEIGHT * 0.4, 
		game->iniFont, 400, game);
}

void MenuLayer::draw() {
	background->draw();
	button->draw();
	text->draw();

	SDL_RenderPresent(game->renderer);
}

void MenuLayer::processControls() {
	// Obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			if (event.type == SDL_QUIT) {
				game->loopActive = false;
			}
			return;
		case SDL_MOUSEBUTTONDOWN:
			mouseToControls(event);
			break;
		case SDL_KEYDOWN:
		default:
			keysToControls(event);
		}
	}

	if (controlContinue) {
		game->layer = game->gameLayer;
		controlContinue = false;
	}
}

void MenuLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		switch (code) {
		case SDLK_SPACE:
			controlContinue = true;
			break;
		}
	}
}

void MenuLayer::mouseToControls(SDL_Event event) {
	float motionX = event.motion.x; // / game->scaleLower;
	float motionY = event.motion.y; // / game->scaleLower;

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (button->button->containsPoint(motionX, motionY)) {
			controlContinue = true;
		}
	}
}