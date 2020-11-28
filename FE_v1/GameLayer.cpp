#include "GameLayer.h"

GameLayer::GameLayer(Game* game) : Layer(game) {
	init();
}

void GameLayer::init() {
	mapManager = new MapManager(this->game);
	
	//Game stuff
	this->turn = 0;

	//Load HUD
	this->turnText = new Text("hola", WIDTH * 0.13, HEIGHT * 0.05, game);
	//this->turnText->content = "Turno: " + to_string(this->turn);
	this->nextTurn();

	loadMap("res/mapa_0.txt");

	//Cargar Characters
	loadCharacters("res/characters.csv", true);
	loadCharacters("res/enemies.csv", false);
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
		manageClickEvent(motionX, motionY);
	}
}

void GameLayer::update() {
	mapManager->update();

	if (mapManager->noUnitsNextToPlay(isPlayerFase())) {
		this->nextTurn();
	}
}

void GameLayer::draw() {
	mapManager->draw();

	// HUD
	//Pintar turno
	turnText->draw();

	//Comprobar si se han de pintar los botones

	SDL_RenderPresent(game->renderer); // Renderiza
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
	cout << mapManager->numberOfColumns << endl;
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
			string delimiter = "\t";

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

	//Añadir el objeto al mapa
	mapManager->addEnemy(character, casilla_x, casilla_y);
}

CharacterClass* GameLayer::obtainCharacterClass(char classCode) {
	CharacterClass* charClass = nullptr;
	switch (classCode) {
	case 'p': {
		cout << "Princess" << endl;
		charClass = new Princess();
		break;
	}
	default:
		break;
	}
	return charClass;
}

void GameLayer::manageClickEvent(float motionX, float motionY) {
	//Encuentra el tile en el que se hizo click
		//y lo devuelve
	vector<int> clickedSquare = mapManager->findClickedSquare(motionX, motionY);
	Tile* tile = mapManager->findClickedTile(clickedSquare);

	//Si tiene que pintar el rango es que tiene seleccionado un character
	if (mapManager->pintarRango) {
		//comprueba si el tile al que se le ha hecho click está en rango
		//	y si no hay un personaje en él
		if (mapManager->isVectorInRange(mapManager->range, clickedSquare)
				&& !mapManager->isCharacterInPosition(clickedSquare)) { //Esto se tendrá que mirar para el caso de healers
			//si lo está mueve el personaje hacia ahí
			mapManager->moveSelectedCharacterTo(clickedSquare);
			mapManager->selectedCharacter->canPlay = false;

			//Termina el turno de ese personaje
			mapManager->deselectRange();
		}
		else {
			mapManager->deselectRange();
		}
	}
	else {
		Character* character = mapManager->findClickedCharacter(clickedSquare);
		if (character != nullptr && character->canPlay) {
			//Prepara el area al que se puede mover para pintarse
			mapManager->setRange(character);

			//Pintar un menú
			//Selecciona opciones que se tienen que pintar
			//Añadelas a la lista de botones a pintar o pon sus booleanos a true
		}
		else {
			mapManager->deselectRange();
		}
	}
}

void GameLayer::nextTurn() {
	this->turn++;
	this->turnText->content = "Turno: " + to_string(this->turn);

	for (auto const& c : mapManager->characters) {
		c->canPlay = isPlayerFase();
	}
	for (auto const& c : mapManager->enemies) {
		c->canPlay = !isPlayerFase();
	}
}

bool GameLayer::isPlayerFase() {
	return turn % 2 == 1;
}
