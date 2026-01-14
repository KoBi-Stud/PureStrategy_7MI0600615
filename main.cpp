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


// Помощни функции за работа с файлове

// Записване на всички потребители във файла
void saveUsers(const std::vector<User>& users) {
	std::ofstream file(DB_FILE.c_str());
	if (!file.is_open()) {
		std::cout << "Error: Could not save users data!" << std::endl;
		return;
	}

	file << users.size() << std::endl;
	for (size_t i = 0; i < users.size(); i++) {
		file << users[i].username << " " << users[i].password << " "
			 << users[i].totalGamesPlayed << " " << users[i].totalGamesWon << std::endl;

		file << users[i].stats.size() << std::endl;
		for (size_t j = 0; j < users[i].stats.size(); j++) {
			file << users[i].stats[j].opponentName << " "
				 << users[i].stats[j].gamesPlayed << " "
				 << users[i].stats[j].gamesWon << std::endl;
		}
	}
	file.close();
}

// Зареждане на потребителите от файла
std::vector<User> loadUsers() {
	std::vector<User> users;
	std::ifstream file(DB_FILE.c_str());

	if (!file.is_open()) {
		return users; // Ако няма файл, връщаме празен вектор
	}

	int usersCount;
	if (!(file >> usersCount)) {
		return users;
	}

	for (int i = 0; i < usersCount; i++) {
		User u;
		file >> u.username >> u.password >> u.totalGamesPlayed >> u.totalGamesWon;

		int statsCount;
		file >> statsCount;
		for (int j = 0; j < statsCount; j++) {
			OpponentStat s;
			file >> s.opponentName >> s.gamesPlayed >> s.gamesWon;
			u.stats.push_back(s);
		}
		users.push_back(u);
	}

	file.close();
	return users;
}

int main() {

	return 0;
}