#pragma once

#include "Actor.h"
#include "Tile.h"
#include "Character.h"
#include "Enemy.h"

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
	void addEnemy(Enemy* character, int xpos, int ypos);
	void deleteCharacter(Character* character);
	void deleteEnemy(Enemy* enemy);
	void pop();
	void setNumberOfColumns(int n);
	void addEnemyRange(vector<int> a);

	vector<int> findClickedSquare(float motionX, float motionY);
	Tile* findClickedTile(vector<int> positions);
	Character* findClickedCharacter(vector<int> positions);
	Enemy* findClickedEnemy(vector<int> positions);

	void setRange(Character* character);
	list<vector<int>> compruebaIrAdyacentes(int x, int y, int coste, MovementType* mt, list<vector<int>> lista, bool isAlly);
	list<vector<int>> compruebaIrAdyacentesAbajo(int x, int y, int coste, MovementType* mt, list<vector<int>> lista, bool isAlly);
	list<vector<int>> addToRange(list<vector<int>> lista, list<vector<int>> lista2);
	void deselectRange();

	vector<int> getCharacterPosition(Character* character);
	vector<int> getEnemyPosition(Enemy* character);
	bool isVectorInRange(list<vector<int>> lista, vector<int> v);
	bool isCharacterInPosition(vector<int> position);
	void moveSelectedCharacterTo(vector<int> square);
	void moveEnemyTo(vector<int> position, Enemy* enemy);
	bool isEnemyInPosition(vector<int> position);

	bool noUnitsNextToPlay(bool playerFase);

	bool enemyInAttackRange();

	map<Enemy*, vector<int>> calculateEnemyFase();
	void realizaAtaque(Character* player, Character* target, map<string, int> result);

	bool areSquaresAdjacent(vector<int> pos1, vector<int> pos2);

	list<vector<int>> calculateAttackRange(Enemy* enemy, list<vector<int>> movementRange);
	bool noEnemyInThatPosition(map<Enemy*, vector<int>> mapEnemies, vector<int> pos);

	Game* game;
	int numberOfColumns;

	//Tiles
	list<Tile*> tiles;
	vector<vector<Tile*>> mapa; //mapa[columnas][filas]
	list<vector<int>> range;
	bool pintarRango = false;
	list<vector<int>> enemyRange;
	bool pintarEnemyRango = false;
	Character* selectedCharacter;
	Enemy* selectedEnemy;
	vector<int> selectedSquare;

	//Characters
	list<Character*> characters;
	map<vector<int>, Character*> characterPositions;
	//Enemies
	list<Enemy*> enemies;
	map<vector<int>, Enemy*> enemyPositions;

private:
	SDL_Texture* marco;
	SDL_Texture* blueSquare;
	SDL_Texture* redSquare;

	void drawBorder(Tile* tile, SDL_Texture* texture, float scrollX = 0);
};

