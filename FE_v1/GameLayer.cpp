#include "GameLayer.h"
#include "ButtonManager.h"

GameLayer::GameLayer(Game* game) : Layer(game) {
	init();
}

void GameLayer::init() {
	mapManager = new MapManager(this->game);
	buttonManager = new ButtonManager(this, this->game);
	resultPanel = new ResultPanel(this->game);

	if (audioBackground != nullptr)
		audioBackground->~Audio();
	audioBackground = new Audio("res/Fire Emblem The Sacred Stones OST - Main Theme.mp3", true);
	audioBackground->play();
	
	//Game stuff
	this->turn = 0;
	this->paintMenu = false;
	this->boolSeleccionaEnemigo = false;
	this->boolResultPanel = false;
	this->boolDescriptionPanel = false;

	//Load HUD
	this->textTurn = new Text("hola", WIDTH * 0.13, HEIGHT * 0.05, game->font, 350, game);
	this->nextTurn();
	this->descriptionPanel = new DescriptionPanel(this->game);

	loadMap("res/mapa_0.txt");

	//Cargar Characters
	loadCharacters("res/characters.csv", true);
	loadCharacters("res/enemies.csv", false);
}

void GameLayer::update() {
	mapManager->update();

	if (mapManager->noUnitsNextToPlay(isPlayerFase())) {
		this->nextTurn();
	}

	if (!isPlayerFase()) {
		//Toda la lógica del turno enemigo
		//Calcula los movimientos que se tienen que hacer
		map<Enemy*, vector<int>> posicionesFinalesEnemigos = mapManager->calculateEnemyFase();

		//Aplica las posiciones finales
		for (auto const& pair : posicionesFinalesEnemigos) {
			mapManager->moveEnemyTo(pair.second, pair.first);
			//cout << "eres el -1?" << endl;
			//cout << pair.first->name << ": " << pair.second[0] << ", " << pair.second[1] << endl;

			if (pair.first->currentHP <= 0)
				mapManager->deleteEnemy(pair.first);
		}

		cout << "enemyPositions.size" << to_string(mapManager->enemyPositions.size()) << endl;

		list<Character*> auxchara(mapManager->characters);
		for (auto const& character : auxchara)
			if (character->currentHP <= 0)
				mapManager->deleteCharacter(character);
	}
}

void GameLayer::draw() {
	mapManager->draw();

	// HUD
	textTurn->draw();

	if (paintMenu)
		buttonManager->draw();

	if (boolResultPanel) {
		resultPanel->draw();
	}

	if (boolDescriptionPanel && 
		(!paintMenu || !boolResultPanel)) {
		descriptionPanel->draw();
	}

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::despintar() {
	mapManager->deselectRange();
	buttonManager->unselectButtonPaint();
	boolSeleccionaEnemigo = false;
	boolResultPanel = false;
}

void GameLayer::processControls() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			game->loopActive = false;
			return;
		}
	}

	mouseToControls(event);
}

void GameLayer::keysToControls(SDL_Event event) {
	//si meto scroll se haría con WASD
}

void GameLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	float motionX = event.motion.x;// / game->scaleLower;
	float motionY = event.motion.y;// / game->scaleLower;

	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (isPlayerFase())
			manageClickEvent(motionX, motionY);
	}
	if (event.type == SDL_MOUSEMOTION) {
		displayDescriptionPanel(motionX, motionY);
	}
}

void GameLayer::displayDescriptionPanel(float motionX, float motionY) {
	vector<int> clickedSquare = mapManager->findClickedSquare(motionX, motionY);

	Character* character = mapManager->findClickedCharacter(clickedSquare);
	Enemy* enemy = mapManager->findClickedEnemy(clickedSquare);
	if (character != nullptr) {
		descriptionPanel->texto->content = character->toString();
		boolDescriptionPanel = true;
	}
	if (enemy != nullptr) {
		descriptionPanel->texto->content = enemy->toString();
		boolDescriptionPanel = true;
	}
	if (character == nullptr && enemy == nullptr) {
		boolDescriptionPanel = false;
	}
}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			mapManager->setNumberOfColumns(line.length());

			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				//cout << "character: " << character << endl;
				float x = 40 / 2 + j * 40; // x central
				float y = 40 + i * 40; // y suelo
				loadMapObject(character, x, y);
			}

			//Por alguna razón repite el último elemento de cada fila, así que debe eliminarse
			mapManager->pop();
		}
	}
	streamFile.close();

	//cout << "mapManager size: " << mapManager->tiles.size() << endl;
	//cout << mapManager->numberOfColumns << endl;
}

void GameLayer::loadMapObject(char character, float x, float y) {
	switch (character) {
	case 'G': {
		GrassTile* tile = new GrassTile(x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		mapManager->addTile(tile);
		break;
	}
	case 'M': {
		MountainTile* tile = new MountainTile(x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		mapManager->addTile(tile);
		break;
	}
	case 'F': {
		ForestTile* tile = new ForestTile(x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		mapManager->addTile(tile);
		break;
	}
	}
}

void GameLayer::loadCharacters(string name, bool characterList) {
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de personajes." << endl;
		return;
	}
	else {
		//Para quitar las cabeceras del medio
		getline(streamFile, line);
		// Por línea
		while (getline(streamFile, line)) {
			istringstream streamLine(line);
			string delimiter = ";";

			size_t pos = 0;
			string token;
			vector<string> data;

			while ((pos = line.find(delimiter)) != string::npos) {
				token = line.substr(0, pos);
				//cout << token << endl;
				//Añade los datos en forma string a una lista
				//para tratarlos más adelante
				data.push_back(token);
				line.erase(0, pos + delimiter.length());
			}//while linea
			//Añadir el último elemento
			data.push_back(line);

			if (characterList) {
				loadCharacterObject(data);
			}
			else {
				loadEnemyObject(data);
			}
		}//while file
	}//else
}

void GameLayer::loadCharacterObject(vector<string> data) {
	//Pasar los datos del vector a variables
	string name = data[0];
	char classCode = data[1].c_str()[0];
	int casilla_x = stoi(data[2]);
	int casilla_y = stoi(data[3]);
	int hp = stoi(data[4]);
	int atk = stoi(data[5]);
	int spd = stoi(data[6]);
	int def = stoi(data[7]);
	int res = stoi(data[8]);

	//Creación del objeto
	//	Obtener la clase del objeto
	CharacterClass* characterClass = this->obtainCharacterClass(classCode);

	//	Transformar casillas a posiciones x, y
	//	(los tiles son de 40x40)
		//posicion de la esquina superior izquierda
	float x = casilla_x * 40;
	float y = casilla_y * 40;
		//pasar de esquina a centro para el Actor
	x = x + 20;
	y = y + 20;

	Character* character = new Character(name, hp, atk, spd, def, res, characterClass,
		x, y, 40, 40, this->game);

	//Añadir el objeto al mapa
	mapManager->addCharacter(character, casilla_x, casilla_y);
}

void GameLayer::loadEnemyObject(vector<string> data) {
	//Pasar los datos del vector a variables
	string name = "enemy_" + data[0];
	char classCode = data[1].c_str()[0];
	int casilla_x = stoi(data[2]);
	int casilla_y = stoi(data[3]);
	int hp = stoi(data[4]);
	int atk = stoi(data[5]);
	int spd = stoi(data[6]);
	int def = stoi(data[7]);
	int res = stoi(data[8]);

	//Creación del objeto
	//	Obtener la clase del objeto
	CharacterClass* characterClass = this->obtainCharacterClass(classCode);

	//	Transformar casillas a posiciones x, y
	//	(los tiles son de 40x40)
		//posicion de la esquina superior izquierda
	float x = casilla_x * 40;
	float y = casilla_y * 40;
	//pasar de esquina a centro para el Actor
	x = x + 20;
	y = y + 20;

	Enemy* character = new Enemy(name, hp, atk, spd, def, res, characterClass,
		x, y, 40, 40, this->game);
	character->canPlay = false;

	//Añadir el objeto al mapa
	mapManager->addEnemy(character, casilla_x, casilla_y);
}

CharacterClass* GameLayer::obtainCharacterClass(char classCode) {
	CharacterClass* charClass = nullptr;
	switch (classCode) {
	case 'r': {
		charClass = new Princess();
		break;
	}
	case 's': {
		charClass = new Soldier();
		break;
	}
	case 'k': {
		charClass = new Knight();
		break;
	}
	case 'a': {
		charClass = new Archer();
		break;
	}
	case 'f': {
		charClass = new Fighter();
		break;
	}
	case 'm': {
		charClass = new Monk();
		break;
	}
	case 'p': {
		charClass = new Pegasus();
		break;
	}
	default:
		charClass = new Princess();
		break;
	}
	return charClass;
}

void GameLayer::manageClickEvent(float motionX, float motionY) {
	//Click en botón
	this->buttonClicked = paintMenu ? buttonManager->click(motionX, motionY) : false;

	//Click en tile si no se hizo click en botón
	if (!buttonClicked) {
		notMenuClick(motionX, motionY);
	}
	else {
		//Se hizo click en un botón así que no debería pintarlos
		buttonManager->unselectButtonPaint();
	}
}

void GameLayer::notMenuClick(float motionX, float motionY) {
	//Encuentra el tile en el que se hizo click y lo devuelve

	vector<int> clickedSquare = mapManager->findClickedSquare(motionX, motionY);

	if (boolResultPanel) {
		if (resultPanel->buttonAttack->button->containsPoint(motionX, motionY)) {
			//Realiza el ataque
			mapManager->realizaAtaque(mapManager->selectedCharacter, 
				mapManager->selectedEnemy, resultPanel->result);

			mapManager->moveSelectedCharacterTo(mapManager->selectedSquare);

			//MapManager no elimina por defecto a los personajes porque no sabe si son Character o Enemy
			if (mapManager->selectedCharacter->currentHP == 0)
				mapManager->deleteCharacter(mapManager->selectedCharacter);
			if (mapManager->selectedEnemy->currentHP == 0)
				mapManager->deleteEnemy(mapManager->selectedEnemy);
		}
		despintar();
	} //Si tiene que pintar el rango es que tiene seleccionado un character
	else if (mapManager->pintarRango) {
		mapClick(clickedSquare);
	}
	else {
		Character* character = mapManager->findClickedCharacter(clickedSquare);
		if (character != nullptr && character->canPlay) {
			//Prepara el area al que se puede mover para pintarse
			mapManager->setRange(character);
		}
		else {
			despintar();
		}
	}
}

void GameLayer::mapClick(vector<int> clickedSquare) {
	Character* character = mapManager->findClickedCharacter(clickedSquare);
	if (boolSeleccionaEnemigo)
		enemyClick(clickedSquare);
	else if (mapManager->isVectorInRange(mapManager->range, clickedSquare)
		&& ( character == nullptr || character == mapManager->selectedCharacter )) {

		//Guarda el selectedTile
		mapManager->selectedSquare = clickedSquare;

		//Pintar un menú
		this->paintMenu = true;

		//Selecciona opciones que se tienen que pintar
		
		//Esperar siempre debe estar activo
		buttonManager->boolWait = true;

		//Si tiene un enemigo en rango añade opción atacar
		if (mapManager->enemyInAttackRange()) {
			buttonManager->boolAttack = true;
			//cout << "enemy in attack range" << endl;
		}
	}
	else {
		despintar();
	}
}

void GameLayer::enemyClick(vector<int> clickedSquare) {
	Enemy* enemy = mapManager->findClickedEnemy(clickedSquare);
	if (enemy != nullptr) {
		//Se hizo click en un enemigo para seleccionar el ataque
		//Mostrar dialogo de resultado con un botón debajo que confirme el ataque
		map<string, int> result = selectedCharacterAttacksEnemy(clickedSquare);
		resultPanel->actualizaText(result, mapManager->selectedCharacter, enemy);
		boolResultPanel = true;
		mapManager->selectedEnemy = enemy;
	}
	else {
		despintar();
	}
}

void GameLayer::nextTurn() {
	this->turn++;
	this->textTurn->content = "Turno: " + to_string(this->turn);
	bool b = isPlayerFase();

	for (auto const& c : mapManager->characters) {
		c->canPlay = b;
	}
	for (auto const& c : mapManager->enemies) {
		c->canPlay = !b;
	}

	/*cout << "EnemyPositions" << endl;
	for (auto const& pair : mapManager->enemyPositions) {
		cout << pair.second->name << ": " << pair.first[0] << ", " << pair.first[1] << endl;
	}*/
}

bool GameLayer::isPlayerFase() {
	return turn % 2 == 1;
}

void GameLayer::moveCharacter(vector<int> clickedSquare) {
	//Mueve el personaje a una casilla
	//Este método se llamará desde las opciones Esperar, Atacar y Curar
	mapManager->moveSelectedCharacterTo(clickedSquare);
	mapManager->selectedCharacter->canPlay = false;

	//Termina el turno de ese personaje
	despintar();
}

map<string, int> GameLayer::selectedCharacterAttacksEnemy(vector<int> clickedSquare) {
	Enemy* enemy = mapManager->findClickedEnemy(clickedSquare);
	if (enemy != nullptr) {
		map<string, int> result = mapManager->selectedCharacter->checkAttack(enemy, 
			mapManager->areSquaresAdjacent(mapManager->selectedSquare, clickedSquare));

		/*for (auto const& pair : result) {
			cout << pair.first << ": " << pair.second << endl;
		}*/

		return result;
	}

	return map<string, int>();
}