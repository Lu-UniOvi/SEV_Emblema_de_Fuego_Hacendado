#pragma once

#include "Actor.h"
#include "Tile.h"
#include "Character.h"

#include <list>
#include <map>
#include <algorithm>


class MapManager {
public:
	MapManager(Game* game);
	void init();
	void draw(float scrollX = 0);
	void update();

	void addTile(Tile* tile);
	void addCharacter(Character* character, int xpos, int ypos);
	void pop();
	void setNumberOfColumns(int n);

	vector<int> findClickedSquare(float motionX, float motionY);
	Tile* findClickedTile(vector<int> positions);
	Character* findClickedCharacter(vector<int> positions);

	void setRange(Character* character);
	list<vector<int>> compruebaIrAdyacentes(int x, int y, int coste, MovementType* mt, list<vector<int>> lista);
	list<vector<int>> compruebaIrAdyacentesAbajo(int x, int y, int coste, MovementType* mt, list<vector<int>> lista);
	list<vector<int>> addToRange(list<vector<int>> lista, list<vector<int>> lista2);
	void deselectRange();

	vector<int> getCharacterPosition(Character* character);
	bool isVectorInRange(list<vector<int>> lista, vector<int> v);
	bool isCharacterInPosition(vector<int> position);
	void moveSelectedCharacterTo(vector<int> square);

	Game* game;
	int numberOfColumns;

	//Tiles
	list<Tile*> tiles;
	vector<vector<Tile*>> mapa; //mapa[columnas][filas]
	list<vector<int>> range;
	bool pintarRango = false;
	Character* selectedCharacter;

	//Characters
	list<Character*> characters;
	map<vector<int>, Character*> characterPositions;

private:
	SDL_Texture* marco;
	SDL_Texture* blueSquare;

	void drawBorder(Tile* tile, SDL_Texture* texture, float scrollX = 0);
};

