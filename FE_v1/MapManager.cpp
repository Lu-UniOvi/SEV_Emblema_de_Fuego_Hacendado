#include "MapManager.h"

MapManager::MapManager(Game* game) {
	this->game = game;
	marco = game->getTexture("res/marco.png");
	blueSquare = game->getTexture("res/blue.png");
	this->pintarRango = false;
}

void MapManager::init() {
	tiles.clear();
	mapa.clear();
}

void MapManager::addTile(Tile* tile) {
	tiles.push_back(tile);
	mapa[mapa.size()-1].push_back(tile);
}

void MapManager::addCharacter(Character* character, int xpos, int ypos) {
	//Add to character list
	characters.push_back(character);
	//Add to map
	vector<int> positions = { xpos, ypos };
	characterPositions[positions] = character;

	cout << "CharacterPositions vector: " << positions[0]
		<< ", " << positions[1] << endl;
}

void MapManager::draw(float scrollX) {
	//Draw Tiles
	for (auto const& tile : tiles)	{
		tile->draw(scrollX);
		this->drawBorder(tile, marco, scrollX);
	}

	if (pintarRango == true) {
		for (auto const& v : this->range) {
			Tile* tile = findClickedTile(v);
			this->drawBorder(tile, blueSquare, scrollX);
		}
	}

	//Draw Characters
	for (auto const& character : characters) {
		character->draw(scrollX);
	}
}

void MapManager::update() {
	for (auto const& character : characters) {
		character->update();
	}
}

void MapManager::drawBorder(Tile* tile, SDL_Texture* texture, float scrollX) {
	SDL_Rect source;
	source.x = 0;
	source.y = 0;
	source.w = tile->fileWidth;
	source.h = tile->fileHeight;

	SDL_Rect destination;
	destination.x = tile->x - tile->width / 2 - scrollX;
	destination.y = tile->y - tile->height / 2;
	destination.w = tile->width;
	destination.h = tile->height;

	SDL_RenderCopyEx(game->renderer, texture, &source, &destination, 0, nullptr, SDL_FLIP_NONE);
}

void MapManager::pop() {
	tiles.pop_back();
	mapa[mapa.size()-1].pop_back();
}

void MapManager::setNumberOfColumns(int n) {
	numberOfColumns = n;
	
	vector<Tile*> v;
	mapa.push_back(v);
}

vector<int> MapManager::findClickedSquare(float motionX, float motionY) {
	//Esto es sin scroll, deberia revisarse si se le añade scroll
	//Los tiles son de tamaño 40x40
	int indexX = motionX / 40;
	int indexY = motionY / 40;

	vector<int> v = {indexX, indexY};

	return v;
}

Tile* MapManager::findClickedTile(vector<int> positions) {
	return mapa[positions[1]][positions[0]];
}

Character* MapManager::findClickedCharacter(vector<int> positions) {
	if (characterPositions.find(positions) != characterPositions.end()) {
		return characterPositions[positions];
	}

	return nullptr;
}


void MapManager::setRange(Character* character) {
	vector<int> characterPosition = this->getCharacterPosition(character);
	
	list<vector<int>> rango = compruebaIrAdyacentes(characterPosition[0], 
		characterPosition[1], 0, character->characterClass->movementType, list<vector<int>>());

	this->range = rango;
	this->pintarRango = true;
}

list<vector<int>> MapManager::compruebaIrAdyacentes(int x, int y, int coste, MovementType* mt, list<vector<int>> lista) {
	lista.push_back({x, y});

	//Comprueba arriba
	if (y - 1 >= 0) {
		Tile* t = mapa[x][y - 1];
		vector<int> v = { x, y - 1 };
		if (mt->costeMovimiento(t) + coste < mt->movementRange
			&& !isVectorInRange(lista, v)) {
			cout << "entro arriba" << endl;
			list<vector<int>> lista2 = compruebaIrAdyacentes(x, y - 1, mt->costeMovimiento(t) + coste, mt, lista);

			for (auto const& a : lista2) {
				if (find(lista.begin(), lista.end(), a) == lista.end())
					lista.push_back(a);
			}
		}
	}
	//Comprueba derecha
	if (x + 1 < mapa.size()) {
		Tile* t = mapa[x + 1][y];
		vector<int> v = { x + 1, y };
		if (mt->costeMovimiento(t) + coste < mt->movementRange
			&& !isVectorInRange(lista, v)) {
			cout << "entro derecha" << endl;
			list<vector<int>> lista2 = compruebaIrAdyacentes(x + 1, y, mt->costeMovimiento(t) + coste, mt, lista);

			for (auto const& a : lista2) {
				if (find(lista.begin(), lista.end(), a) == lista.end())
					lista.push_back(a);
			}
		}
	}
	//Comprueba izquierda
	if (x - 1 >= 0) {
		Tile* t = mapa[x - 1][y];
		vector<int> v = { x - 1, y };
		if (mt->costeMovimiento(t) + coste < mt->movementRange
			&& !isVectorInRange(lista, v)) {
			cout << "entro izquierda" << endl;
			list<vector<int>> lista2 = compruebaIrAdyacentes(x - 1, y, mt->costeMovimiento(t) + coste, mt, lista);

			for (auto const& a : lista2) {
				if (find(lista.begin(), lista.end(), a) == lista.end())
					lista.push_back(a);
			}
		}
	}
	//Comprueba abajo
	if (y + 1 < mapa[0].size()) {
		Tile* t = mapa[x][y + 1];
		vector<int> v = { x, y + 1 };
		if (mt->costeMovimiento(t) + coste < mt->movementRange
			&& !isVectorInRange(lista, v)) {
			cout << "entro abajo" << endl;
			list<vector<int>> lista2 = compruebaIrAdyacentes(x, y + 1, mt->costeMovimiento(t) + coste, mt, lista);

			for (auto const& a : lista2) {
				if (find(lista.begin(), lista.end(), a) == lista.end())
					lista.push_back(a);
			}
		}
	}

	return lista;
}

vector<int> MapManager::getCharacterPosition(Character* character) {
	for (auto const& pair : characterPositions) {
		if (pair.second == character) {
			return pair.first;
		}
	}

	return vector<int>();
}

bool MapManager::isVectorInRange(list<vector<int>> lista, vector<int> v1) {
	for (auto const& v2 : lista) {
		if (v1[0] == v2[0] && v1[1] == v2[1])
			return true;
	}
	return false;
}