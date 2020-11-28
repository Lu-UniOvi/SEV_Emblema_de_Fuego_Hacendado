#pragma once

#include "Game.h"
#include "Layer.h"

#include "Tile.h"
#include "GrassTile.h"
#include "MountainTile.h"
#include "ForestTile.h"

#include "Character.h"
#include "CharacterClass.h"
#include "Princess.h"

#include "MapManager.h"

#include "Text.h"

#include <fstream>
#include <sstream>
#include <list>

class GameLayer : public Layer {
public:
	GameLayer(Game* game);

	void init() override;

	void processControls() override;
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event);

	void update() override;
	void draw() override;

	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void loadCharacters(string name, bool characterList);
	void loadCharacterObject(vector<string> data);
	void loadEnemyObject(vector<string> data);
	CharacterClass* obtainCharacterClass(char classCode);

	void manageClickEvent(float motionX, float motionY);

	void nextTurn();
	bool isPlayerFase();

	//Map
	int mapWidth;
	MapManager* mapManager;

	//Game control
	int turn;

	//HUD
	Text* turnText;
};

