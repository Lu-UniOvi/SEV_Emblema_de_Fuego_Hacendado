#include "Game.h"
#include "GameLayer.h"

Game::Game() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error SDL_Init" << SDL_GetError() << endl;
	}
	if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) {
		cout << "Error Window y Renderer" << SDL_GetError() << endl;
	}
	SDL_SetWindowTitle(window, "El Emblema de Fuego de Hacendado");
	// Escalado de imágenes de calidad 
	// https://wiki.libsdl.org/SDL_HINT_RENDER_SCALE_QUALITY
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	gameLayer = new GameLayer(this);
	layer = gameLayer;

	// fuentes
	TTF_Init();
	font = TTF_OpenFont("res/overload.ttf", 28);

	loopActive = true; // bucle activo
	loop();
}

void Game::loop() {
	int initTick; // ms de inicio loop
	int endTick; // ms de fin de loop
	int differenceTick; // fin - inicio
	while (loopActive) {
		initTick = SDL_GetTicks();

		// Controles
		layer->processControls();
		// Actualizar elementos
		layer->update();
		// Dibujar
		layer->draw();

		endTick = SDL_GetTicks();
		differenceTick = endTick - initTick;

		if (differenceTick < (1000 / 30)) {
			SDL_Delay((1000 / 30) - differenceTick);
		}
	}
}

SDL_Texture* Game::getTexture(string filename) {
	if (mapTextures.find(filename) != mapTextures.end()) {
		//cout << "retorno recurso cacheado" << filename << endl;
	}
	else {
		//cout << "Nuevo hay que cachearlo " << filename << endl;
		SDL_Surface* surface = IMG_Load(filename.c_str());
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		mapTextures[filename] = texture;
	}

	return mapTextures[filename];
}