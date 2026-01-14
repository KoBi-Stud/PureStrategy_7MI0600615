/**
*
* Solution to course project # 1
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Konstantin Bilalov
* @idnumber 7MI0600615
* @compiler GCC
*
* <Main implementation of the Pure Strategy card game>
*
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib> // for rand, srand for shuffle
#include <ctime>   // time for shuffle seed

// Структура за статистика срещу конкретен опонент
struct OpponentStat {
	std::string opponentName;
	int gamesPlayed;
	int gamesWon;
};

// Структура за потребител
struct User {
	std::string username;
	std::string password;
	int totalGamesPlayed;
	int totalGamesWon;
	std::vector<OpponentStat> stats;
};

// Глобални константи за точките на картите
const int CARD_ACE = 1;
const int CARD_JACK = 11;
const int CARD_QUEEN = 12;
const int CARD_KING = 13;

// Път до файла с базата данни
const std::string DB_FILE = "users_data.txt";

int main() {

	return 0;
}