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
#include "Soldier.h"
#include "Knight.h"
#include "Archer.h"
#include "Fighter.h"
#include "Monk.h"
#include "Pegasus.h"

#include "MapManager.h"

#include "Text.h"
#include "Button.h"
//#include "ButtonManager.h"
#include "ButtonManagerParent.h"
#include "ResultPanel.h"
#include "DescriptionPanel.h"

#include "Audio.h"

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
	void displayDescriptionPanel(float motionX, float motionY);

	void update() override;
	void draw() override;
	void despintar();

	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void loadCharacters(string name, bool characterList);
	void loadCharacterObject(vector<string> data);
	void loadEnemyObject(vector<string> data);
	CharacterClass* obtainCharacterClass(char classCode);

	void manageClickEvent(float motionX, float motionY);
	void notMenuClick(float motionX, float motionY);
	void mapClick(vector<int> clickedSquare);
	void enemyClick(vector<int> clickedSquare);

	void nextTurn();
	bool isPlayerFase();
	void moveCharacter(vector<int> clickedSquare);
	map<string, int> selectedCharacterAttacksEnemy(vector<int> clickedSquare);

	//Map
	int mapWidth;
	MapManager* mapManager;

	//Game control
	int turn;
	bool buttonClicked;
	bool boolSeleccionaEnemigo;
	bool boolResultPanel;
	bool boolDescriptionPanel;

	//HUD
	Text* textTurn;
	bool paintMenu;

	ButtonManagerParent* buttonManager;
	ResultPanel* resultPanel;
	DescriptionPanel* descriptionPanel;

	Audio* audioBackground;
};