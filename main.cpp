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

// Проверка дали потребителско име съществува
bool userExists(const std::vector<User>& users, const std::string& username) {
	for (size_t i = 0; i < users.size(); i++) {
		if (users[i].username == username) {
			return true;
		}
	}
	return false;
}

// Намиране на индекс на потребител (или -1 ако го няма)
int findUserIndex(const std::vector<User>& users, const std::string& username, const std::string& password) {
	for (size_t i = 0; i < users.size(); i++) {
		if (users[i].username == username && users[i].password == password) {
			return i;
		}
	}
	return -1;
}

// Функция за визуален разделител за естетика
void printSeparator() {
	std::cout << "========================================" << std::endl;
}

// Регистриране на нов потребител
void registerUser(std::vector<User>& users) {
	std::string username, password;
	printSeparator();
	std::cout << "--- REGISTRATION ---" << std::endl;
	std::cout << "Enter username: ";
	std::cin >> username;

	if (userExists(users, username)) {
		std::cout << "Error: Username already exists!" << std::endl;
		return;
	}

	std::cout << "Enter password: ";
	std::cin >> password;

	User newUser;
	newUser.username = username;
	newUser.password = password;
	newUser.totalGamesPlayed = 0;
	newUser.totalGamesWon = 0;

	users.push_back(newUser);
	saveUsers(users); // Веднага записваме във файла
	std::cout << "Registration successful!" << std::endl;
}

// Вход в системата (връща индекса на потребителя в масива/вектора)
int loginUser(const std::vector<User>& users) {
	std::string username, password;
	printSeparator();
	std::cout << "--- LOGIN ---" << std::endl;
	std::cout << "Enter username: ";
	std::cin >> username;
	std::cout << "Enter password: ";
	std::cin >> password;

	int index = findUserIndex(users, username, password);
	if (index != -1) {
		std::cout << "Login successful! Welcome, " << users[index].username << "!" << std::endl;
		return index;
	}
	else {
		std::cout << "Error: Invalid username or password!" << std::endl;
		return -1;
	}
}

// Извеждане на статистика за потребител
void printStats(const User& u) {
	printSeparator();
	std::cout << "STATS FOR: " << u.username << std::endl;
	std::cout << "Total games played: " << u.totalGamesPlayed << std::endl;

	double winRate = 0;
	if (u.totalGamesPlayed > 0) {
		winRate = (double)u.totalGamesWon / u.totalGamesPlayed * 100.0;
	}
	std::cout << "Total games won: " << u.totalGamesWon << " (" << winRate << "%)" << std::endl;

	std::cout << "Details vs Opponents:" << std::endl;
	if (u.stats.empty()) {
		std::cout << "  No games played against specific opponents yet." << std::endl;
	}
	else {
		for (size_t i = 0; i < u.stats.size(); i++) {
			double vsRate = 0;
			if (u.stats[i].gamesPlayed > 0) {
				vsRate = (double)u.stats[i].gamesWon / u.stats[i].gamesPlayed * 100.0;
			}
			std::cout << "  Vs " << u.stats[i].opponentName << ": "
				 << u.stats[i].gamesPlayed << " played ("
				 << u.stats[i].gamesWon << "/" << vsRate << "% won)" << std::endl;
		}
	}
	printSeparator();
}


// ФУНКЦИИ ЗА ИГРАТА

// Генериране на ново тесте (числа 1-13)
std::vector<int> generateDeck() {
	std::vector<int> deck;
	for (int i = 1; i <= 13; i++) {
		deck.push_back(i);
	}
	return deck;
}

// Алгоритъм на Fisher-Yates за разбъркване
void mySwap (int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}
void shuffleDeck(std::vector<int>& deck) {
	for (size_t i = 0; i < deck.size(); i++) {
		int j = i + rand() % (deck.size() - i);
		mySwap(deck[i], deck[j]);
	}
}

// Изчистване на екрана (чрез нови редове, за да не се виждат действията на другия играч)
void clearScreen() {
	for (int i = 0; i < 50; i++) {
		std::cout << std::endl;
	}
}

// Премахване на карта от ръката на играча
void removeCard(std::vector<int>& hand, int card) {
	for (size_t i = 0; i < hand.size(); i++) {
		if (hand[i] == card) {
			hand.erase(hand.begin() + i);
			break;
		}
	}
}

// Проверка дали картата я има в ръката
bool hasCard(const std::vector<int>& hand, int card) {
	for (size_t i = 0; i < hand.size(); i++) {
		if (hand[i] == card) return true;
	}
	return false;
}

int main() {

	return 0;
}