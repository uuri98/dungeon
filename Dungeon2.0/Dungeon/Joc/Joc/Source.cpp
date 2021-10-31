#include <iostream>
#include <string>
#include "Utils.h"
#include <time.h>

enum enemys { GOBLIN,TROLL,WIZARD, DRAGON, LASTENEMY };

char map[11][11] = {{'.','#','#','#','#','#','#','#','#','#','#'},
					{'#','@','.','G','P','#','B','#','B','D','#'},
					{'#','_','#','.','#','.','.','#','.','.','#'},
					{'#','.','#','K','#','.','W','|','.','P','#'},
					{'#','G','#','#','T','#','.','#','#','#','#'},
					{'#','.','#','W','.','.','.','|','.','B','#'},
					{'#','S','.','P','#','#','T','#','W','.','#'},
					{'#','.','#','.','#','M','#','B','.','P','#'},
					{'#','.','#','.','#','W','#','.','#','#','#'},
					{'#','B','#','T','.','.','.','W','.','K','#'},
					{'#','#','#','#','#','#','#','#','#','#','#'}};

struct EnemyInfo {
	std::string name;
	int max_life;
	int cur_life;
	int dmg;
	std::string attack;
	
};

struct PlayerInfo {
	std::string name;
	int max_life = 100;
	int cur_life = 100;
	int dmg;
	int swordN;
	int bombN=4;
	int potionN;
	int keyN;
	int pos_x;
	int pos_y;
	int map;
};

PlayerInfo player;
bool combatMode = false;
int enemyMaxhp, enemyCurhp,enemyDmg;
int boss_pos_x, boss_pos_y;
std::string currentEnemyName;
std::string currentEnemyAttack;
EnemyInfo enemies[LASTENEMY];

void initPlayer() {
	std::cout << "Welcome adventurer! What is your name?\n";
	std::cin >> player.name;
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			if (map[i][j] == '@') {
				player.pos_x = j;
				player.pos_y = i;
				map[i][j] = '.';
			}
		}
	}
}

void initEnemys() {
	
	enemies[GOBLIN].name = "Goblin";
	enemies[GOBLIN].max_life = 20;
	enemies[GOBLIN].cur_life = 20;
	enemies[GOBLIN].dmg = 5 + rand() % 8 + 1;
	enemies[GOBLIN].attack = "Slash";

	enemies[TROLL].name = "Troll";
	enemies[TROLL].max_life = 40;
	enemies[TROLL].cur_life = 40;
	enemies[TROLL].dmg = 10 + rand() % 8 + 1;
	enemies[TROLL].attack = "Butting";

	enemies[WIZARD].name = "Wizard";
	enemies[WIZARD].max_life = 60;
	enemies[WIZARD].cur_life = 60;
	enemies[WIZARD].dmg = 15 + rand() % 10 + 1;
	enemies[WIZARD].attack = "Fire Explosion";

	enemies[DRAGON].name = "Zu: The fire dragon";
	enemies[DRAGON].max_life = 200;
	enemies[DRAGON].cur_life = 200;
	enemies[DRAGON].dmg = 15 + rand() % 10 + 1;
	enemies[DRAGON].attack = "Fire Breath";
}

void changeTile() {

	int j = player.pos_x;
	int i = player.pos_y;

	if (map[i][j] == 'P') {
		map[i][j] = '.';
	}
	else if (map[i][j] == 'B') {
		map[i][j] = '.';
	}
	else if (map[i][j] == 'S') {
		map[i][j] = '.';
	}
	else if (map[i][j] == 'K') {
		map[i][j] = '.';
	}
	else if (map[i][j] == 'G') {
		map[i][j] = '.';
	}
	else if (map[i][j] == 'T') {
		map[i][j] = '.';
	}
	else if (map[i][j] == 'W') {
		map[i][j] = '.';
	}
	else if (map[i][j] == '_') {
		map[i][j] = '.';
	}
	else if (map[i][j] == 'M') {
		map[i][j] = '.';
	}
}

bool collisionMap(int pos_x, int pos_y) {
	bool ret_val = false;
	if (pos_x < 0 || pos_x >= 11 || pos_y < 0 || pos_y >= 11) {
		ret_val = true;
	}
	else if (map[pos_y][pos_x] == '#') {
		ret_val = true;
	}
	else if (map[pos_y-1][pos_x] == '_' || map[pos_y][pos_x-1] == '|') {
		ret_val = true;
	}
	return ret_val;
}

void checkSurroudings() {
	std::cout << "You can go ";

	if (collisionMap(player.pos_x,player.pos_y - 1) == false ) {
		std::cout<<"[NORTH]";
	}
	if (collisionMap(player.pos_x, player.pos_y + 1) == false) {
		std::cout << "[SOUTH]";
	}
	if (collisionMap(player.pos_x - 1, player.pos_y) == false) {
		std::cout << "[WEST]";
	}
	if (collisionMap(player.pos_x + 1, player.pos_y) == false) {
		std::cout << "[EAST]";
	}
	std::cout << std::endl;
}

std::string getObject(int pos_x, int pos_y) {

	if (map[pos_y][pos_x] == 'B') {
		return "Bomb";
	}
	else if (map[pos_y][pos_x] == 'P') {
		return "Potion";
	}
	else if (map[pos_y][pos_x] == 'S') {
		return "Sword";
	}
	else if (map[pos_y][pos_x] == 'K') {
		return "Key";
	}
	else if (map[pos_y][pos_x] == '_' || map[pos_y][pos_x] == '|') {
		return "Locked Door";
	}
	else if (map[pos_y][pos_x] == 'M') {
		return "Map";
	}
	return "NOTHING";
}

bool hasObject(int pos_x, int pos_y) {
	bool ret_value = false;
	if (pos_y < 0 || pos_y>11 || pos_x < 0 || pos_x>11) {
		ret_value = false;
	}
	else if (map[pos_y][pos_x] == 'B' || map[pos_y][pos_x] == 'P' || map[pos_y][pos_x] == 'S' || map[pos_y][pos_x] == 'K' || map[pos_y][pos_x] == '_' || map[pos_y][pos_x] == 'M' || map[pos_y][pos_x] == '|') {
		ret_value = true;
	}
	return ret_value;
}

void objectSurroundings() {
	std::string object;

	if (hasObject(player.pos_x, player.pos_y)) {
		object = getObject(player.pos_x, player.pos_y);
		if (object == "Locked Door") {
			std::cout << "You approached the door... It is locked. Maybe a key will open it...\n";
		}
		else if (object == "Map") {
			std::cout << "You found a map! Pick it to unlock the map mode!\n";
		}
		else {
			std::cout << "There is a " << object << " on the floor" << std::endl;
		}
		
	}
	else if (hasObject(player.pos_x, player.pos_y + 1)) {
		object = getObject(player.pos_x, player.pos_y + 1);
		std::cout << "There is a " << object << " south" << std::endl;
	}
	else if (hasObject(player.pos_x, player.pos_y - 1)) {
		object = getObject(player.pos_x, player.pos_y - 1);
		std::cout << "There is a " << object << " north" << std::endl;
	}
	else if (hasObject(player.pos_x + 1, player.pos_y)) {
		object = getObject(player.pos_x + 1, player.pos_y);
		std::cout << "There is a " << object << " east" << std::endl;
	}
	else if (hasObject(player.pos_x - 1, player.pos_y)) {
		object = getObject(player.pos_x - 1, player.pos_y);
		std::cout << "There is a " << object << " west" << std::endl;
	}
}

std::string getEnemy(int pos_x, int pos_y) {

	if (map[pos_y][pos_x] == 'G') {
		return "Goblin";
	}
	else if (map[pos_y][pos_x] == 'T') {
		return "Troll";
	}
	else if (map[pos_y][pos_x] == 'W') {
		return "Wizard";
	}
	else if (map[pos_y][pos_x] == 'D') {
		return "Zu: The fire dragon";
	}
	return "NOTHING";
}

bool hasEnemy(int pos_x, int pos_y) {
	bool ret_value = false;
	if (pos_y < 0 || pos_y>11 || pos_x < 0 || pos_x>11) {
		ret_value = false;
	}
	else if (map[pos_y][pos_x] == 'G' || map[pos_y][pos_x] == 'T' || map[pos_y][pos_x] == 'W' || map[pos_y][pos_x] == 'D') {
		ret_value = true;
	}
	return ret_value;
}

void enemySurroundings() {
	std::string enemy;

	if (hasEnemy(player.pos_x, player.pos_y)) {
		enemy = getEnemy(player.pos_x, player.pos_y);
		std::cout << "You stepped into a " << enemy << std::endl;
	}
	else if (hasEnemy(player.pos_x, player.pos_y + 1)) {
		enemy = getEnemy(player.pos_x, player.pos_y + 1);
		std::cout << "There is a " << enemy << " south" << std::endl;
	}
	else if (hasEnemy(player.pos_x, player.pos_y - 1)) {
		enemy = getEnemy(player.pos_x, player.pos_y - 1);
		std::cout << "There is a " << enemy << " north" << std::endl;
	}
	else if (hasEnemy(player.pos_x + 1, player.pos_y)) {
		enemy = getEnemy(player.pos_x + 1, player.pos_y);
		std::cout << "There is a " << enemy << " east" << std::endl;
	}
	else if (hasEnemy(player.pos_x - 1, player.pos_y)) {
		enemy = getEnemy(player.pos_x - 1, player.pos_y);
		std::cout << "There is a " << enemy << " west" << std::endl;
	}
}

bool enemyOnTile() {
	
	if (hasEnemy(player.pos_x, player.pos_y)) {
		combatMode = true;
		return combatMode;
	}
	else {
		combatMode = false;
		return combatMode;
	}
}

void curEnemy() {

	if (getEnemy(player.pos_x, player.pos_y) == "Goblin") {
		enemyMaxhp = enemies[GOBLIN].max_life;
		enemyCurhp = enemies[GOBLIN].cur_life;
		enemyDmg = enemies[GOBLIN].dmg;
		currentEnemyName = enemies[GOBLIN].name;
		currentEnemyAttack = enemies[GOBLIN].attack;
	}
	else if (getEnemy(player.pos_x, player.pos_y) == "Troll") {
		enemyMaxhp = enemies[TROLL].max_life;
		enemyCurhp = enemies[TROLL].cur_life;
		enemyDmg = enemies[TROLL].dmg;
		currentEnemyName = enemies[TROLL].name;
		currentEnemyAttack = enemies[TROLL].attack;
	}
	else if (getEnemy(player.pos_x, player.pos_y) == "Wizard") {
		enemyMaxhp = enemies[WIZARD].max_life;
		enemyCurhp = enemies[WIZARD].cur_life;
		enemyDmg = enemies[WIZARD].dmg;
		currentEnemyName = enemies[WIZARD].name;
		currentEnemyAttack = enemies[WIZARD].attack;
	}
	else if (getEnemy(player.pos_x, player.pos_y) == "Zu: The fire dragon") {
		enemyMaxhp = enemies[DRAGON].max_life;
		enemyCurhp = enemies[DRAGON].cur_life;
		enemyDmg = enemies[DRAGON].dmg;
		currentEnemyName = enemies[DRAGON].name;
		currentEnemyAttack = enemies[DRAGON].attack;
	}

}

void searchBoss() {
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			if (map[i][j] == 'D') {
				boss_pos_x = j;
				boss_pos_y = i;
			}
		}
	}
}

void whereIsBoss() {
	if (player.pos_x == boss_pos_x) {
		if (player.pos_y == boss_pos_y) {
			std::cout << "Finnally..You are in front of the devil Dragon. It is bigger than i thought..\n";
		}
		else if (player.pos_y > boss_pos_y) {
			std::cout << "You can feel a devil aura coming from [NORTH]...\n";
		}
		else if (player.pos_y < boss_pos_y) {
			std::cout << "You can feel a devil aura coming from [SOUTH]...\n";
		}

	}
	else if (player.pos_y == boss_pos_y) {
		if (player.pos_x < boss_pos_x) {
			std::cout << "You can feel a devil aura coming from [EAST]...\n";
		}
		else if (player.pos_x > boss_pos_x) {
			std::cout << "You can feel a devil aura coming from [WEST]...\n";
		}

	}
	else if (player.pos_x<boss_pos_x && player.pos_y>boss_pos_y) {
		std::cout << "You can feel a devil aura coming from [NORTHEAST]...\n";
	}
	else if (player.pos_x > boss_pos_x && player.pos_y > boss_pos_y) {
		std::cout << "You can feel a devil aura coming from [NORTHWEST]...\n";
	}
	else if (player.pos_x < boss_pos_x && player.pos_y < boss_pos_y) {
		std::cout << "You can feel a devil aura coming from [SOUTHEAST]...\n";
	}
	else if (player.pos_x > boss_pos_x && player.pos_y < boss_pos_y) {
		std::cout << "You can feel a devil aura coming from [SOUTHWEST]...\n";
	}
}

void finalBoss() {
	if (currentEnemyName == "Zu: The fire dragon") {

		if (enemyCurhp == 200) {
			std::cout << "You finally made it to Zu: The fire dragon! He is the boss of the dungeon for a reason.. so be careful! Good luck!\n";
		}
	}
}

void initCombatMap() {
	std::cout << "-----------------------\n";
	std::cout << "[" << player.name << "] -- HP (" << player.cur_life << "/" << player.max_life << ")" << std::endl;
	std::cout << "VS" << std::endl;
	std::cout << "[" << currentEnemyName << "] -- HP (" << enemyCurhp << "/" << enemyMaxhp << ")" << std::endl;
	std::cout << "-----------------------\n";
}

void combatDmgs(){
	
	if (player.dmg == 0) {
		player.cur_life -= enemyDmg;
		enemyCurhp -= player.dmg;
		if (player.cur_life > 100) {
			player.cur_life = 100;
		}
	}
	else{
		player.cur_life -= enemyDmg;
		enemyCurhp -= player.dmg;
		std::cout << "You attacked " << currentEnemyName << " causing " << player.dmg << std::endl;
	}
	
}

void showStatus() {
	std::cout << std::endl << "------------STATUS-----------" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::cout << player.name << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "HP: " << player.cur_life << " / " << player.max_life << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "Sword: " << player.swordN << std::endl;
	std::cout << "Potion: " << player.potionN << std::endl;
	std::cout << "Bomb: " << player.bombN << std::endl;
	std::cout << "Key: " << player.keyN << std::endl;
}

void showWelcome() {

	std::cout << "Right " << player.name << "! You are one step away from entering the dungeon... Before entering, I have to warn you!\n"; 
	std::cout << "Through this path, you will face dangerous enemies you've never seen and you will discover what truly fear is. So think twice before entering. \n";
	std::cout << "Oh, and once you enter, you can't get out until defeat the final boss. \nI see... So you decided... You really are a fool! Well, i will explain you the basics then.\n";
	std::cout << "There are some useful tools you can call any time during your path.\nIf you type 'status', a table with your inventory, name and hp will appear.\n"; 
	std::cout << "By typing 'help', some instructions about how to move around the dungeon, and anything you can need will show.\nI think thats all! Are you ready? Good Luck!\n\n\n";
}

void showMap() {
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			if (player.pos_y == i && player.pos_x == j) {
				std::cout << '@';
			}
			else {
				std::cout << map[i][j];
			}
		}
		std::cout << std::endl;
	}
}

void showHelp() {

}

int main() {
	srand(time(NULL));
	initPlayer();
	showWelcome();
	initEnemys();
	searchBoss();
	bool game_over = false;
	std::string input = "";
	while (!game_over) {
		checkSurroudings();
		whereIsBoss();
		objectSurroundings();
		enemySurroundings();
		enemyOnTile();
		curEnemy();
		if (!combatMode) {
			std::cout << "What are you going to do? \n> ";
			std::getline(std::cin, input);
			//Tratar el input
			std::vector< std::string > split_input = splitString(input, ' ');
			if (split_input.size() > 0) {
				if (split_input.size() == 2) {
					if (split_input[0] == "go") {
						if (split_input[1] == "north") {
							if (!collisionMap(player.pos_x, player.pos_y - 1)) {
								player.pos_y--;
								std::cout << player.name << " went north" << std::endl;
							}
							else {
								std::cout << "You can't go this way!\n";
							}
						}
						else if (split_input[1] == "south") {
							if (!collisionMap(player.pos_x, player.pos_y + 1)) {
								player.pos_y++;
								std::cout << player.name << " went south" << std::endl;
							}
							else {
								std::cout << "You can't go this way!\n";
							}
						}
						else if (split_input[1] == "east") {
							if (!collisionMap(player.pos_x + 1, player.pos_y)) {
								player.pos_x++;
								std::cout << player.name << " went east" << std::endl;
							}
							else {
								std::cout << "You can't go this way!\n";
							}
						}
						else if (split_input[1] == "west") {
							if (!collisionMap(player.pos_x - 1, player.pos_y)) {
								player.pos_x--;
								std::cout << player.name << " went west" << std::endl;
							}
							else {
								std::cout << "You can't go this way!\n";
							}
						}
						else {
							std::cout << "Go where?" << std::endl;
						}
					}
					else if (split_input[0] == "pick") {
						if (split_input[1] == "potion") {
							if (getObject(player.pos_x, player.pos_y) == "Potion") {
								player.potionN++;
								std::cout << "You picked a potion.\n";
								changeTile();
							}

						}
						else if (split_input[1] == "sword") {
							if (getObject(player.pos_x, player.pos_y) == "Sword") {
								player.swordN++;
								std::cout << "You picked a sword.\n";
								changeTile();
							}
						}
						else if (split_input[1] == "bomb") {
							if (getObject(player.pos_x, player.pos_y) == "Bomb") {
								player.bombN++;
								std::cout << "You picked a bomb.\n";
								changeTile();
							}
						}
						else if (split_input[1] == "key") {
							if (getObject(player.pos_x, player.pos_y) == "Key") {
								player.keyN++;
								std::cout << "You picked a key.\n";
								changeTile();
							}
						}
						else if (split_input[1] == "map") {
							if (getObject(player.pos_x, player.pos_y) == "Map") {
								player.map++;
								std::cout << "You picked a map. Type use map to see the dungeon from above!\n";
								changeTile();
							}
						}
						else {
							std::cout << "Pick what?" << std::endl;
						}
					}
					else if (split_input[0] == "use") {
						if (split_input[1] == "potion") {
							player.cur_life += 80 + rand() % 20 + 1;
							std::cout << "You used a potion!" << std::endl;
							if (player.cur_life > 100) {
								player.cur_life = 100;
							}
							else {
								std::cout << "You don't have any potions!\n";
							}
						}
						else if (split_input[1] == "map") {
							if (player.map > 0) {
								showMap();
							}
							else {
								std::cout << "You don't have a map!\n";
							}
						}
						else if (split_input[1] == "key") {
							if(getObject(player.pos_x, player.pos_y) == "Locked Door"){
								if (player.keyN > 0) {
								std::cout << "It seems the key worked! The door opens for you!\n";
								changeTile();
									}
								else {
								std::cout << "You don't have any key!\n";
									}
							}
							else {
								std::cout << "You have to be standing at the door to use it!\n";
							}
							
						}
						else {
							std::cout << "Use what?" << std::endl;
						}
					}
				}
				else if (split_input[0] == "help") {
					showHelp();
				}
				else if (split_input[0] == "status") {
					showStatus();
				}
				else {
					std::cout << "Incorrect number of words" << std::endl;
				}
			}
		}
		else if (combatMode) {
			while (combatMode) {
				finalBoss();
				initCombatMap();
				std::cout << currentEnemyName << " attacks you with " << currentEnemyAttack << ". It deals you "<< enemyDmg << " dmg."<<std::endl;
				std::cout << "What are you going to do? \n> ";
				std::getline(std::cin, input);
				std::vector< std::string > split_input = splitString(input, ' ');
					if (split_input.size() == 2) {
						if (split_input[0] == "use") {
							if (split_input[1] == "sword") {
								if (player.swordN > 0) {
									player.dmg = 30;
								}
								else {
									std::cout << "You dont have any swords!\n";
								}
							}
							else if (split_input[1] == "bomb") {
								if (player.bombN > 0) {
									player.dmg = 60;
									player.bombN--;
								}
								else {
									std::cout << "You dont have any bombs!\n";
								}
							}
							else if (split_input[1] == "potion") {
								if (player.potionN > 0) {
									player.cur_life += 80 + rand()%20+1;
									player.potionN--;
									player.dmg = 0;
									std::cout << "You used a potion!\n";
								}
								else {
									std::cout << "You dont have any potions!\n";
								}
							}
							else if (split_input[1] == "fists") {
								player.dmg =5 + rand() % 10 + 1;
							}
							else {
								std::cout << "Use what?\n";
							}
							combatDmgs();
						}
						else {
							std::cout << "Use what?\n";
						}
					}
					else {
						std::cout << "Incorrect number of words!\n";
					}

				if (player.cur_life <= 0) {
					std::cout << "Seems like you died...\n";
					std::cout << "------------GAME OVER----------\n";
					system("pause");
					return 0;
				}
				else if (enemyCurhp <= 0) {
					if (enemyMaxhp == 200) {
						std::cout << "You defeated Zu! Your name will never be forgotten!\n";
						std::cout << "                                 _\n";
						std::cout << "                                | |\n";
						std::cout << "  ___ ___  _ __   __ _ _ __ __ _| |_ ___\n";
						std::cout << " / __/ _ \\| '_ \\ / _` | '__/ _` | __/ __| \n";
						std::cout << "| (_| (_) | | | | (_| | | | (_| | |_\\__ \\ \n";
						std::cout << " \\___\\___/|_| |_|\\__, |_|  \\__ _|\\__|___/ \n";
						std::cout << "                 __/ |\n";
						std::cout << "                |___ /\n";
						system("pause");
						return 0;
					}
					combatMode = false;
					std::cout << currentEnemyName << " died!\n";
					changeTile();

				}
				system("pause");
				system("cls");
			}
		}

		system("pause");
		system("cls"); //Limpiar consola
	}
	system("pause");
	return 0;
}