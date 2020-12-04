#include "MapManager.h"

MapManager::MapManager(Game* game) {
	this->game = game;
	this->marco = game->getTexture("res/marco.png");
	this->blueSquare = game->getTexture("res/blue.png");
	this->redSquare = game->getTexture("res/red.png");

	this->pintarRango = false;
	this->selectedCharacter = nullptr;
	this->selectedSquare = {-1,-1};
}

void MapManager::init() {
	tiles.clear();
	mapa.clear();
	range.clear();
	enemyRange.clear();
	characters.clear();
	characterPositions.clear();

	this->pintarRango = false;
	this->selectedCharacter = nullptr;
}

void MapManager::draw(float scrollX) {
	//Draw Tiles
	for (auto const& tile : tiles) {
		tile->draw(scrollX);
		this->drawBorder(tile, marco, scrollX);
	}

	if (pintarRango) {
		for (auto const& v : this->range) {
			Tile* tile = findClickedTile(v);
			if (tile != nullptr)
				this->drawBorder(tile, blueSquare, scrollX);
		}
		if (pintarEnemyRango) {
			for (auto const& v : this->enemyRange) {
				Tile* tile = findClickedTile(v);
				if (tile != nullptr)
					this->drawBorder(tile, redSquare, scrollX);
			}
		}
	}

	//Draw Characters
	for (auto const& character : characters) {
		character->draw(scrollX);
	}

	//Draw Enemies
	for (auto const& enemy : enemies) {
		enemy->draw(scrollX);
	}
}

void MapManager::update() {
	for (auto const& character : characters) {
		character->update();
	}
	for (auto const& enemy : enemies) {
		enemy->update();
	}
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

	//Tile* t = findClickedTile(positions);
	//t->setCharacter(character);

	//cout << "Tile->Char " << t->character->name << endl;
}

void MapManager::addEnemy(Enemy* character, int xpos, int ypos) {
	enemies.push_back(character);

	vector<int> positions = { xpos, ypos };
	enemyPositions[positions] = character;
	//cout << character->name << ": " << positions[0] << ", " << positions[1] << endl;
}

void MapManager::deleteCharacter(Character* character) {
	characters.remove(character);

	vector<int> removePosition = { -1,-1 };

	for (auto const& pair : characterPositions) {
		if (pair.second == character)
			removePosition = pair.first;
	}

	if (removePosition[0] != -1)
		characterPositions.erase(removePosition);
}

void MapManager::deleteEnemy(Enemy* enemy) {
	enemies.remove(enemy);

	vector<int> removePosition = { -1,-1 };

	for (auto const& pair : enemyPositions) {
		if (pair.second == enemy)
			removePosition = pair.first;
	}

	if (removePosition[0] != -1)
		enemyPositions.erase(removePosition);
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

void MapManager::addEnemyRange(vector<int> a) {
	bool add = true;
	
	for (auto const& b : enemyRange) {
		if (b == a)
			add = false;
	}

	if (add)
		enemyRange.push_back(a);
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
	//cout << "findClickedTile: " << positions[1] << ", " << positions[0] << endl;
	if (positions[1] >= 0 && positions[1] < mapa.size() && 
		positions[0] >= 0 && positions[0] < mapa[0].size())
		return mapa[positions[1]][positions[0]];
	return nullptr;
}

Character* MapManager::findClickedCharacter(vector<int> positions) {
	if (characterPositions.find(positions) != characterPositions.end()) {
		return characterPositions[positions];
	}
	return nullptr;
}

Enemy* MapManager::findClickedEnemy(vector<int> positions) {
	if (enemyPositions.find(positions) != enemyPositions.end()) {
		return enemyPositions[positions];
	}
	return nullptr;
}


void MapManager::setRange(Character* character) {
	vector<int> characterPosition = this->getCharacterPosition(character);
	
	list<vector<int>> rango = compruebaIrAdyacentes(characterPosition[0], 
		characterPosition[1], 0, character->characterClass->movementType, list<vector<int>>(), character->isAlly());
	rango = this->addToRange(rango, compruebaIrAdyacentesAbajo(characterPosition[0],
		characterPosition[1], 0, character->characterClass->movementType, list<vector<int>>(), character->isAlly()));

	this->range = rango;
	this->pintarRango = true;
	this->selectedCharacter = character;
}

list<vector<int>> MapManager::compruebaIrAdyacentes(int x, int y, int coste, MovementType* mt, list<vector<int>> lista, bool isAlly) {
	lista.push_back({x, y});

	//Comprueba arriba
	if (y - 1 >= 0) {
		Tile* t = mapa[y - 1][x];
		vector<int> v = { x, y - 1 };
		bool notObstacle = isAlly ? findClickedEnemy(v) == nullptr : findClickedCharacter(v) == nullptr;
		if (mt->costeMovimiento(t) + coste < mt->movementRange
			&& !isVectorInRange(lista, v) && notObstacle) {
			list<vector<int>> lista2 = compruebaIrAdyacentes(x, y - 1, mt->costeMovimiento(t) + coste, mt, lista, isAlly);

			for (auto const& a : lista2) {
				if (find(lista.begin(), lista.end(), a) == lista.end())
					lista.push_back(a);
			}
		}
	}
	//Comprueba derecha
	if (x + 1 < mapa[0].size()) {
		//cout << "compruebaDerecha: " << to_string(x + 1) << endl;
		Tile* t = mapa[y][x + 1];
		vector<int> v = { x + 1, y };
		bool notObstacle = isAlly ? findClickedEnemy(v) == nullptr : findClickedCharacter(v) == nullptr;
		if (mt->costeMovimiento(t) + coste < mt->movementRange
			&& !isVectorInRange(lista, v) && notObstacle) {
			list<vector<int>> lista2 = compruebaIrAdyacentes(x + 1, y, mt->costeMovimiento(t) + coste, mt, lista, isAlly);

			for (auto const& a : lista2) {
				if (find(lista.begin(), lista.end(), a) == lista.end())
					lista.push_back(a);
			}
		}
	}
	//Comprueba izquierda
	if (x - 1 >= 0) {
		Tile* t = mapa[y][x - 1];
		vector<int> v = { x - 1, y };
		bool notObstacle = isAlly ? findClickedEnemy(v) == nullptr : findClickedCharacter(v) == nullptr;
		if (mt->costeMovimiento(t) + coste < mt->movementRange
			&& !isVectorInRange(lista, v) && notObstacle) {
			list<vector<int>> lista2 = compruebaIrAdyacentes(x - 1, y, mt->costeMovimiento(t) + coste, mt, lista, isAlly);

			for (auto const& a : lista2) {
				if (find(lista.begin(), lista.end(), a) == lista.end())
					lista.push_back(a);
			}
		}
	}
	//Comprueba abajo
	if (y + 1 < mapa.size()) {
		Tile* t = mapa[y + 1][x];
		vector<int> v = { x, y + 1 };
		bool notObstacle = isAlly ? findClickedEnemy(v) == nullptr : findClickedCharacter(v) == nullptr;
		if (mt->costeMovimiento(t) + coste < mt->movementRange
			&& !isVectorInRange(lista, v) && notObstacle) {
			list<vector<int>> lista2 = compruebaIrAdyacentes(x, y + 1, mt->costeMovimiento(t) + coste, mt, lista, isAlly);

			for (auto const& a : lista2) {
				if (find(lista.begin(), lista.end(), a) == lista.end())
					lista.push_back(a);
			}
		}
	}

	return lista;
}

list<vector<int>> MapManager::compruebaIrAdyacentesAbajo(int x, int y, int coste, MovementType* mt, list<vector<int>> lista, bool isAlly) {
	lista.push_back({ x, y });

	//Comprueba abajo
	if (y + 1 < mapa.size()) {
		Tile* t = mapa[y + 1][x];
		vector<int> v = { x, y + 1 };
		bool notObstacle = isAlly ? findClickedEnemy(v) == nullptr : findClickedCharacter(v) == nullptr;
		if (mt->costeMovimiento(t) + coste < mt->movementRange
			&& !isVectorInRange(lista, v) && notObstacle) {
			list<vector<int>> lista2 = compruebaIrAdyacentesAbajo(x, y + 1, mt->costeMovimiento(t) + coste, mt, lista, isAlly);

			for (auto const& a : lista2) {
				if (find(lista.begin(), lista.end(), a) == lista.end())
					lista.push_back(a);
			}
		}
	}
	//Comprueba derecha
	if (x + 1 < mapa[0].size()) {
		Tile* t = mapa[y][x + 1];
		vector<int> v = { x + 1, y };
		bool notObstacle = isAlly ? findClickedEnemy(v) == nullptr : findClickedCharacter(v) == nullptr;
		if (mt->costeMovimiento(t) + coste < mt->movementRange
			&& !isVectorInRange(lista, v) && notObstacle) {
			list<vector<int>> lista2 = compruebaIrAdyacentesAbajo(x + 1, y, mt->costeMovimiento(t) + coste, mt, lista, isAlly);

			for (auto const& a : lista2) {
				if (find(lista.begin(), lista.end(), a) == lista.end())
					lista.push_back(a);
			}
		}
	}
	//Comprueba izquierda
	if (x - 1 >= 0) {
		Tile* t = mapa[y][x - 1];
		vector<int> v = { x - 1, y };
		bool notObstacle = isAlly ? findClickedEnemy(v) == nullptr : findClickedCharacter(v) == nullptr;
		if (mt->costeMovimiento(t) + coste < mt->movementRange
			&& !isVectorInRange(lista, v) && notObstacle) {
			list<vector<int>> lista2 = compruebaIrAdyacentesAbajo(x - 1, y, mt->costeMovimiento(t) + coste, mt, lista, isAlly);

			for (auto const& a : lista2) {
				if (find(lista.begin(), lista.end(), a) == lista.end())
					lista.push_back(a);
			}
		}
	}

	return lista;
}

list<vector<int>> MapManager::addToRange(list<vector<int>> lista, list<vector<int>> lista2) {
	for (auto const& a : lista2) {
		if (find(lista.begin(), lista.end(), a) == lista.end())
			lista.push_back(a);
	}
	return lista;
}

void MapManager::deselectRange() {
	this->pintarRango = false;
	this->selectedCharacter = nullptr;
	this->selectedSquare = {-1,-1};
	this->enemyRange.clear();
}

vector<int> MapManager::getCharacterPosition(Character* character) {
	for (auto const& pair : characterPositions) {
		if (pair.second == character) {
			return pair.first;
		}
	}
	return { -1, -1 };
}

vector<int> MapManager::getEnemyPosition(Enemy* character) {
	for (auto const& pair : enemyPositions) {
		if (pair.second == character) {
			return pair.first;
		}
	}
	return {-1, -1};
}

bool MapManager::isVectorInRange(list<vector<int>> lista, vector<int> v1) {
	for (auto const& v2 : lista) {
		if (v1[0] == v2[0] && v1[1] == v2[1])
			return true;
	}
	return false;
}

bool MapManager::isCharacterInPosition(vector<int> position) {
	return findClickedCharacter(position) != nullptr;
}

void MapManager::moveSelectedCharacterTo(vector<int> position) {
	//Encontrar el character en characterPositions
	vector<int> originalPos = this->getCharacterPosition(this->selectedCharacter);
	
	if (this->findClickedCharacter(originalPos) != nullptr) {
		//Eliminar esa referencia en characterPositions
		characterPositions.erase(originalPos);
		//Añadir nueva pareja vector,character
		characterPositions[position] = this->selectedCharacter;

		//Actualizar la posicion del character
		Tile* tile = this->findClickedTile(position);
		if (tile != nullptr) {
			this->selectedCharacter->x = tile->x;
			this->selectedCharacter->y = tile->y;
		}
	}
}

void MapManager::moveEnemyTo(vector<int> position, Enemy* enemy) {
	//Encontrar el character en characterPositions
	vector<int> originalPos = getEnemyPosition(enemy);
	
	if (findClickedEnemy(originalPos) != nullptr) {
		//Eliminar esa referencia en enemyPositions
		enemyPositions.erase(originalPos);
		//Añadir nueva pareja vector,character
		enemyPositions[position] = enemy;

		//Actualizar la posicion del character
		Tile* tile = this->findClickedTile(position);
		if (tile != nullptr) {
			enemy->x = tile->x;
			enemy->y = tile->y;
		}
	}
}

bool MapManager::isEnemyInPosition(vector<int> position) {
	return findClickedEnemy(position) != nullptr;
}

bool MapManager::noUnitsNextToPlay(bool playerFase) {
	if (playerFase) {
		for (auto const& c : characters) {
			if (c->canPlay)
				return false;
		}
	}
	else {
		for (auto const& c : enemies) {
			if (c->canPlay)
				return false;
		}
	}
	return true;
}

bool MapManager::enemyInAttackRange() {
	vector<int> no = { -1,-1 };
	bool isEnemy = false;
	if (selectedCharacter != nullptr && selectedSquare != no) {
		//CloseRange
		if (selectedCharacter->characterClass->weaponType->closeRange) {
			//Comprueba casillas adyacentes
			vector<int> top = selectedSquare;
			top[1] = selectedSquare[1] - 1;
			vector<int> right = selectedSquare;
			right[0] = selectedSquare[0] + 1;
			vector<int> bottom = selectedSquare;
			bottom[1] = selectedSquare[1] + 1;
			vector<int> left = selectedSquare;
			left[0] = selectedSquare[0] - 1;

			if (isEnemyInPosition(top)) {
				addEnemyRange(top);
				isEnemy = true;
			}
			if (isEnemyInPosition(bottom)) {
				addEnemyRange(bottom);
				isEnemy = true;
			}
			if (isEnemyInPosition(left)) {
				addEnemyRange(left);
				isEnemy = true;
			}
			if (isEnemyInPosition(right)) {
				addEnemyRange(right);
				isEnemy = true;
			}
		}
		//LongRange
		if (selectedCharacter->characterClass->weaponType->longRange) {
			//Comprueba casillas a una casilla de distancia, contando diagonales
			vector<int> top = selectedSquare;
			top[1] = selectedSquare[1] - 2;
			vector<int> topright = top;
			topright[0] = selectedSquare[0] + 1;
			vector<int> right = selectedSquare;
			right[0] = selectedSquare[0] + 2;
			vector<int> bottomright = right;
			bottomright[1] = selectedSquare[1] + 1;
			vector<int> bottom = selectedSquare;
			bottom[1] = selectedSquare[1] + 2;
			vector<int> bottomleft = bottom;
			bottomleft[0] = selectedSquare[0] - 1;
			vector<int> left = selectedSquare;
			left[0] = selectedSquare[0] - 2;
			vector<int> topleft = top;
			topleft[0] = selectedSquare[0] - 1;

			if (isEnemyInPosition(top)) {
				addEnemyRange(top);
				isEnemy = true;
			}
			else if (isEnemyInPosition(topright)) {
				addEnemyRange(topright);
				isEnemy = true;
			}
			else if (isEnemyInPosition(right)) {
				addEnemyRange(right);
				isEnemy = true;
			}
			else if (isEnemyInPosition(bottomright)) {
				addEnemyRange(bottomright);
				isEnemy = true;
			}
			else if (isEnemyInPosition(bottom)) {
				addEnemyRange(bottom);
				isEnemy = true;
			}
			else if (isEnemyInPosition(bottomleft)) {
				addEnemyRange(bottomleft);
				isEnemy = true;
			}
			else if (isEnemyInPosition(left)) {
				addEnemyRange(left);
				isEnemy = true;
			}
			else if (isEnemyInPosition(topleft)) {
				addEnemyRange(topleft);
				isEnemy = true;
			}
		}
	}

	pintarEnemyRango = isEnemy;

	return isEnemy;
}

map<Enemy*, vector<int>> MapManager::calculateEnemyFase() {
	map<Enemy*, vector<int>> posicionesFinalesEnemigos;
	for (auto const& enemy : enemies) {
		if (enemy->canPlay) {
			//Conseguir su rango de acción
			vector<int> enemyPosition = this->getEnemyPosition(enemy);

			list<vector<int>> movementRange = compruebaIrAdyacentes(enemyPosition[0],
				enemyPosition[1], 0, enemy->characterClass->movementType, list<vector<int>>(), enemy->isAlly());
			movementRange = this->addToRange(movementRange, compruebaIrAdyacentesAbajo(enemyPosition[0],
				enemyPosition[1], 0, enemy->characterClass->movementType, list<vector<int>>(), enemy->isAlly()));

			// TODO
			//De momento hagamos como que los enemigos solo pueden atacar a casillas que pueden moverse.
			/*enemy->characterClass->movementType->movementRange +=
				enemy->characterClass->weaponType->longRange ? 2 : 1;*/

			list<vector<int>> attackRange = calculateAttackRange(enemy, movementRange);

			//Puedo o hacer lo mismo aumentando el movement range y volviendolo a poner como estaba despues de conseguir el rango
			//O me curro otro metodo
			//O paso de todo y los enemigos son inferiores porque anime powers

			//Obtener los personajes que se pueden atacar
			map<Character*, vector<int>> charactersInRange;
			for (auto const& pos : attackRange) {
				Character* chara = findClickedCharacter(pos);
				if (chara != nullptr)
					charactersInRange[chara] = pos;
			}

			if (charactersInRange.size() != 0) {
				//Calcular el resultado de atacar a los personajes en rango
				map<Character*, map<string, int>> resultados;
				for (auto const& pair : charactersInRange) {
					bool enemyCloseAttackRange = enemy->characterClass->weaponType->closeRange;

					//Si el enemigo puede atacar a closeRange y el character no puede contraatacar
					//y el enemy puede estar en una casilla adyacente
					if (enemy->characterClass->weaponType->closeRange &&
						!pair.first->characterClass->weaponType->closeRange)
						enemyCloseAttackRange = true;
					//Si el enemigo puede atacar a longRange y el character no puede contraatacar
					//y el enemy puede estar en una casilla a distancia
					if (enemy->characterClass->weaponType->longRange &&
						!pair.first->characterClass->weaponType->longRange)
						enemyCloseAttackRange = false;

					resultados[pair.first] = enemy->checkAttack(pair.first,
						enemyCloseAttackRange);
				}
				//Escoger el mejor resultado
				map<string, int> mejor;
				Character* mejorChar = nullptr;
				mejor["damageDealt"] = -1;
				mejor["damageTaken"] = 2147483647;
				for (auto const& pair : resultados) {
					if (pair.second.at("damageDealt") > mejor["damageDealt"]
						|| pair.second.at("damageTaken") < mejor["damageTaken"]) {
						mejor = pair.second;
						mejorChar = pair.first;
					}
				}

				//Selecciona la casilla a la que se moverá el enemigo para realizar el ataque
				vector<int> posicionMejor = charactersInRange[mejorChar];
				//Encuentra una casilla en la que se pueda posicionar el enemigo para atacar
				if (enemy->characterClass->weaponType->closeRange) {
					vector<int> top = posicionMejor;
					top[1] = posicionMejor[1] - 1;
					vector<int> right = posicionMejor;
					right[0] = posicionMejor[0] + 1;
					vector<int> bottom = posicionMejor;
					bottom[1] = posicionMejor[1] + 1;
					vector<int> left = posicionMejor;
					left[0] = posicionMejor[0] - 1;

					if (top[1] >= 0 && isVectorInRange(movementRange, top)
						&& !isEnemyInPosition(top)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, top))
						posicionesFinalesEnemigos[enemy] = top;
					else if (right[0] < mapa[0].size() && isVectorInRange(movementRange, right)
						&& !isEnemyInPosition(right)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, right))
						posicionesFinalesEnemigos[enemy] = right;
					else if (bottom[1] < mapa.size() && isVectorInRange(movementRange, bottom)
						&& !isEnemyInPosition(bottom)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, bottom))
						posicionesFinalesEnemigos[enemy] = bottom;
					else if (left[0] >= 0 && isVectorInRange(movementRange, left)
						&& !isEnemyInPosition(left)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, left))
						posicionesFinalesEnemigos[enemy] = left;
				}
				if (enemy->characterClass->weaponType->longRange) {
					vector<int> top = posicionMejor;
					top[1] = posicionMejor[1] - 1;
					vector<int> topright = top;
					topright[0] = posicionMejor[0] + 1;
					vector<int> right = posicionMejor;
					right[0] = posicionMejor[0] + 1;
					vector<int> bottomright = right;
					bottomright[1] = posicionMejor[1] + 1;
					vector<int> bottom = posicionMejor;
					bottom[1] = posicionMejor[1] + 1;
					vector<int> bottomleft = bottom;
					bottomleft[0] = posicionMejor[0] - 1;
					vector<int> left = posicionMejor;
					left[0] = posicionMejor[0] - 1;
					vector<int> topleft = top;
					topleft[0] = posicionMejor[0] - 1;

					if (top[1] >= 0 && isVectorInRange(movementRange, top)
						&& !isEnemyInPosition(top)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, top))
						posicionesFinalesEnemigos[enemy] = top;
					else if (topright[0] < mapa[0].size() && topright[1] >= 0 && isVectorInRange(movementRange, topright)
						&& !isEnemyInPosition(topright)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, topright))
						posicionesFinalesEnemigos[enemy] = topright;
					else if (right[0] < mapa[0].size() && isVectorInRange(movementRange, right)
						&& !isEnemyInPosition(right)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, right))
						posicionesFinalesEnemigos[enemy] = right;
					else if (bottomright[0] < mapa[0].size() && bottomright[1] < mapa.size() 
						&& isVectorInRange(movementRange, bottomright)
						&& !isEnemyInPosition(bottomright)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, bottomright))
						posicionesFinalesEnemigos[enemy] = bottomright;
					else if (bottom[1] < mapa.size() && isVectorInRange(movementRange, bottom)
						&& !isEnemyInPosition(bottom)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, bottom))
						posicionesFinalesEnemigos[enemy] = bottom;
					else if (bottomleft[0] >= 0 && bottomleft[1] < mapa.size() && isVectorInRange(movementRange, bottomleft)
						&& !isEnemyInPosition(bottomleft)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, bottomleft))
						posicionesFinalesEnemigos[enemy] = bottomleft;
					else if (left[0] >= 0 && isVectorInRange(movementRange, left)
						&& !isEnemyInPosition(left)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, left))
						posicionesFinalesEnemigos[enemy] = left;
					else if (topleft[0] >= 0 && topleft[1] >= 0 && isVectorInRange(movementRange, topleft)
						&& !isEnemyInPosition(topleft)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, topleft))
						posicionesFinalesEnemigos[enemy] = topleft;
				}
				/*cout << enemy->name << " attacks " << mejorChar->name << endl;
				cout << " moves to " << to_string(posicionesFinalesEnemigos[enemy][0])
					<< ", " << posicionesFinalesEnemigos[enemy] [1] << endl;*/
				//Aplicar el ataque
				realizaAtaque(enemy, mejorChar, mejor);
			}
			else { //Si no hay personajes en rango 
				//Calcular la distancia a todos los personajes
				map<vector<int>, int> distancias;
				for (auto const& pair : characterPositions) {
					distancias[pair.first] = abs(enemyPosition[0] - pair.first[0])
						+ abs(enemyPosition[1] - pair.first[1]);
				}
				//Escoger el más cercano
				vector<int> closest = {-1, -1};
				int distancia = 2147483647;
				for (auto const& pair : distancias) {
					if (pair.second < distancia
						&& !findClickedEnemy(pair.first)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, pair.first)) {
						distancia = pair.second;
						closest = pair.first;
					}
				}
				// TODO
				//Encontrar casilla dentro del rango del enemigo más cercana al player más cercano
				map<vector<int>, int> distanciasEnemyTarget; //posicion, distancia
				for (auto const& pos : movementRange) {
					distanciasEnemyTarget[pos] = abs(closest[0] - pos[0])
						+ abs(closest[1] - pos[1]);
				}

				vector<int> closestInRange = { -1, -1 };
				int distanciaInRange = 2147483647;
				for (auto const& pair : distanciasEnemyTarget) {
					if (pair.second < distanciaInRange
						&& !findClickedEnemy(pair.first)
						&& noEnemyInThatPosition(posicionesFinalesEnemigos, pair.first)) {
						distanciaInRange = pair.second;
						closestInRange = pair.first;
					}
				}

				posicionesFinalesEnemigos[enemy] = closestInRange;

				enemy->canPlay = false;

				/*cout << enemy->name << " moves" << endl;
				cout << " moves to " << to_string(posicionesFinalesEnemigos[enemy][0])
					<< ", " << posicionesFinalesEnemigos[enemy][1] << endl;*/


				/*//Encontrar casilla adyacente al Character que esté en rango del enemigo
				vector<int> aux = { closest[0], closest[1] - 1 }; //top
				posicionesFinalesEnemigos[enemy] = {-1,-1};
				if (find(movementRange.begin(), movementRange.end(), aux) != movementRange.end()) {
					posicionesFinalesEnemigos[enemy] = aux;
				}
				aux = { closest[0] + 1, closest[1] }; //right
				if (find(movementRange.begin(), movementRange.end(), aux) != movementRange.end()
					&& posicionesFinalesEnemigos[enemy][0] != -1) {
					posicionesFinalesEnemigos[enemy] = aux;
				}
				aux = { closest[0], closest[1] + 1 }; //bottom
				if (find(movementRange.begin(), movementRange.end(), aux) != movementRange.end()
					&& posicionesFinalesEnemigos[enemy][0] != -1) {
					posicionesFinalesEnemigos[enemy] = aux;
				}
				aux = { closest[0] - 1, closest[1] }; //left
				if (find(movementRange.begin(), movementRange.end(), aux) != movementRange.end()
					&& posicionesFinalesEnemigos[enemy][0] != -1) {
					posicionesFinalesEnemigos[enemy] = aux;
				}*/
			}
		}
	}

	return posicionesFinalesEnemigos;
}

void MapManager::realizaAtaque(Character* player, Character* target, map<string, int> result) {
	player->currentHP =
		player->currentHP > result["damageTaken"] ?
		player->currentHP - result["damageTaken"] : 0;
	target->currentHP =
		target->currentHP > result["damageDealt"] ?
		target->currentHP - result["damageDealt"] : 0;

	player->canPlay = false;
}

bool MapManager::areSquaresAdjacent(vector<int> pos1, vector<int> pos2) {
	bool adjacent = false;

	if (pos1[0] == pos2[0] && pos1[1] + 1 == pos2[1])
		adjacent = true;
	if (pos1[0] == pos2[0] && pos1[1] - 1 == pos2[1])
		adjacent = true;
	if (pos1[0] + 1 == pos2[0] && pos1[1] == pos2[1])
		adjacent = true;
	if (pos1[0] - 1 == pos2[0] && pos1[1] == pos2[1])
		adjacent = true;

	return adjacent;
}

list<vector<int>> MapManager::calculateAttackRange(Enemy* enemy, list<vector<int>> movementRange) {
	list<vector<int>> attackRange(movementRange);

	for (auto const& pos : movementRange) {
		//CloseRange
		if (enemy->characterClass->weaponType->closeRange) {
			//Comprueba casillas adyacentes
			vector<int> top = pos;
			top[1] = pos[1] - 1;
			vector<int> right = pos;
			right[0] = pos[0] + 1;
			vector<int> bottom = pos;
			bottom[1] = pos[1] + 1;
			vector<int> left = pos;
			left[0] = pos[0] - 1;

			if (!isVectorInRange(attackRange, top)) {
				attackRange.push_back(top);
			}
			if (!isVectorInRange(attackRange, bottom)) {
				attackRange.push_back(bottom);
			}
			if (!isVectorInRange(attackRange, left)) {
				attackRange.push_back(left);
			}
			if (!isVectorInRange(attackRange, right)) {
				attackRange.push_back(right);
			}
		}
		//LongRange
		if (enemy->characterClass->weaponType->longRange) {
			//Comprueba casillas a una casilla de distancia, contando diagonales
			vector<int> top = pos;
			top[1] = pos[1] - 2;
			vector<int> topright = top;
			topright[0] = pos[0] + 1;
			vector<int> right = pos;
			right[0] = pos[0] + 2;
			vector<int> bottomright = right;
			bottomright[1] = pos[1] + 1;
			vector<int> bottom = pos;
			bottom[1] = pos[1] + 2;
			vector<int> bottomleft = bottom;
			bottomleft[0] = pos[0] - 1;
			vector<int> left = pos;
			left[0] = pos[0] - 2;
			vector<int> topleft = top;
			topleft[0] = pos[0] - 1;

			if (!isVectorInRange(attackRange, top)) {
				attackRange.push_back(top);
			}
			else if (!isVectorInRange(attackRange, topright)) {
				attackRange.push_back(topright);
			}
			else if (!isVectorInRange(attackRange, right)) {
				attackRange.push_back(right);
			}
			else if (!isVectorInRange(attackRange, bottomright)) {
				attackRange.push_back(bottomright);
			}
			else if (!isVectorInRange(attackRange, bottom)) {
				attackRange.push_back(bottom);
			}
			else if (!isVectorInRange(attackRange, bottomleft)) {
				attackRange.push_back(bottomleft);
			}
			else if (!isVectorInRange(attackRange, left)) {
				attackRange.push_back(left);
			}
			else if (!isVectorInRange(attackRange, topleft)) {
				attackRange.push_back(topleft);
			}
		}
	}

	return attackRange;
}

bool MapManager::noEnemyInThatPosition(map<Enemy*, vector<int>> mapEnemies, vector<int> pos) {
	for (auto const& pair : mapEnemies) {
		if (pair.second[0] == pos[0] && pair.second[1] == pos[1])
			return false;
	}

	return true;
}
