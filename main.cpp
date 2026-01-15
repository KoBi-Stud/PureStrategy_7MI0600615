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
const int COUNT_OF_CARDS = 13;

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

// Избор на карта от играч
int getPlayerCard(const std::string& playerName, const std::vector<int>& hand, int prizeCard, int pendingCount) {
	int card = 0;
	while (true) {
		std::cout << "\n--- Turn: " << playerName << " ---" << std::endl;
		std::cout << "Prize card on table: " << prizeCard << " points." << std::endl;
		if (pendingCount > 0) {
			std::cout << "BONUS: There are " << pendingCount << " more prize cards from previous ties!" << std::endl;
		}

		std::cout << "Your available cards: ";
		for (size_t i = 0; i < hand.size(); i++) {
			std::cout << hand[i] << " ";
		}
		std::cout << std::endl;

		std::cout << "Choose a card to play (enter value 1-13 from which are available): ";
		std::cin >> card;

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			std::cout << "Invalid input! Enter a number." << std::endl;
			continue;
		}

		if (hasCard(hand, card)) {
			break;
		}
		else {
			std::cout << "You don't have that card or it is invalid! Try again." << std::endl;
		}
	}
	return card;
}

// Обновяване на статистиката след игра
void updateGameStats(User& winner, User& loser) {
	// Обновяваме общата статистика
	winner.totalGamesPlayed++;
	winner.totalGamesWon++;
	loser.totalGamesPlayed++;

	// Обновяваме статистиката срещу конкретния опонент за победителя
	bool found = false;
	for (size_t i = 0; i < winner.stats.size(); i++) {
		if (winner.stats[i].opponentName == loser.username) {
			winner.stats[i].gamesPlayed++;
			winner.stats[i].gamesWon++;
			found = true;
			break;
		}
	}
	if (!found) {
		OpponentStat newStat;
		newStat.opponentName = loser.username;
		newStat.gamesPlayed = 1;
		newStat.gamesWon = 1;
		winner.stats.push_back(newStat);
	}

	// Обновяваме статистиката срещу конкретния опонент за загубилия
	found = false;
	for (size_t i = 0; i < loser.stats.size(); i++) {
		if (loser.stats[i].opponentName == winner.username) {
			loser.stats[i].gamesPlayed++;
			// Тук не увеличаваме gamesWon
			found = true;
			break;
		}
	}
	if (!found) {
		OpponentStat newStat;
		newStat.opponentName = winner.username;
		newStat.gamesPlayed = 1;
		newStat.gamesWon = 0;
		loser.stats.push_back(newStat);
	}
}

// Обновяване на статистиката при равенство
void updateDrawStats(User& p1, User& p2) {
	// 1. Обновяваме общите игри
	p1.totalGamesPlayed++;
	p2.totalGamesPlayed++;

	// При равенство увеличаваме само played games без победи

	// 2. Обновяваме статистиката на Първия срещу Втория
	bool found = false;
	for (size_t i = 0; i < p1.stats.size(); i++) {
		if (p1.stats[i].opponentName == p2.username) {
			p1.stats[i].gamesPlayed++;
			found = true;
			break;
		}
	}
	if (!found) {
		OpponentStat newStat;
		newStat.opponentName = p2.username;
		newStat.gamesPlayed = 1;
		newStat.gamesWon = 0; // Няма победа
		p1.stats.push_back(newStat);
	}

	// 3. Обновяваме статистиката на Втория срещу Първия
	found = false;
	for (size_t i = 0; i < p2.stats.size(); i++) {
		if (p2.stats[i].opponentName == p1.username) {
			p2.stats[i].gamesPlayed++;
			found = true;
			break;
		}
	}
	if (!found) {
		OpponentStat newStat;
		newStat.opponentName = p1.username;
		newStat.gamesPlayed = 1;
		newStat.gamesWon = 0; // Няма победа
		p2.stats.push_back(newStat);
	}
}

// Основна логика на рундовете
void playGame(User& p1, User& p2) {
	std::vector<int> deckP1 = generateDeck();
	std::vector<int> deckP2 = generateDeck();
	std::vector<int> deckPrize = generateDeck();

	shuffleDeck(deckPrize);

	int scoreP1 = 0;
	int scoreP2 = 0;
	std::vector<int> pendingPrizes; // За натрупване при равенство

	printSeparator();
	std::cout << "GAME STARTED: " << p1.username << " VS " << p2.username << std::endl;
	std::cout << "Rules: A=1, J=11, Q=12, K=13. Highest card takes the prize!" << std::endl;

	for (size_t i = 0; i < COUNT_OF_CARDS; i++) {
		int currentPrize = deckPrize[i];
		pendingPrizes.push_back(currentPrize);

		printSeparator();
		std::cout << "ROUND " << (i + 1) << "/13" << std::endl;
		std::cout << "Current prize card value: " << currentPrize << std::endl;
		if (pendingPrizes.size() > 1) {
			std::cout << "!!! TIE BREAKER !!! Winner takes " << pendingPrizes.size() << " cards!" << std::endl;
		}

		// Ход на играч 1
		std::cout << "\nPress Enter for " << p1.username << " to play...";
		std::cin.ignore(); std::cin.get(); // Пауза
		int cardP1 = getPlayerCard(p1.username, deckP1, currentPrize, pendingPrizes.size() - 1);
		removeCard(deckP1, cardP1);
		clearScreen(); // Скриваме избора

		// Ход на играч 2
		std::cout << "\nPress Enter for " << p2.username << " to play...";
		// cin.get(); // Тук може да не трябва втора пауза ако буферът е чист, но е безопасно
		int cardP2 = getPlayerCard(p2.username, deckP2, currentPrize, pendingPrizes.size() - 1);
		removeCard(deckP2, cardP2);
		clearScreen();

		// Резултат от рунда
		std::cout << "--- ROUND RESULT ---" << std::endl;
		std::cout << p1.username << " played: " << cardP1 << std::endl;
		std::cout << p2.username << " played: " << cardP2 << std::endl;

		if (cardP1 > cardP2) {
			std::cout << "Winner: " << p1.username << "!" << std::endl;
			int roundPoints = 0;
			for (size_t k = 0; k < pendingPrizes.size(); k++) roundPoints += pendingPrizes[k];
			scoreP1 += roundPoints;
			std::cout << p1.username << " gains " << roundPoints << " points." << std::endl;
			pendingPrizes.clear();
		}
		else if (cardP2 > cardP1) {
			std::cout << "Winner: " << p2.username << "!" << std::endl;
			int roundPoints = 0;
			for (size_t k = 0; k < pendingPrizes.size(); k++) roundPoints += pendingPrizes[k];
			scoreP2 += roundPoints;
			std::cout << p2.username << " gains " << roundPoints << " points." << std::endl;
			pendingPrizes.clear();
		}
		else {
			std::cout << "IT'S A TIE! Prize remains for the next round." << std::endl;
			// Картите остават в pendingPrizes
		}

		std::cout << "Current Score -> " << p1.username << ": " << scoreP1 << " | " << p2.username << ": " << scoreP2 << std::endl;
	}

	// Край на играта
	printSeparator();
	std::cout << "GAME OVER!" << std::endl;
	std::cout << "Final Score -> " << p1.username << ": " << scoreP1 << " | " << p2.username << ": " << scoreP2 << std::endl;

	if (scoreP1 > scoreP2) {
		std::cout << "WINNER: " << p1.username << std::endl;
		updateGameStats(p1, p2);
	}
	else if (scoreP2 > scoreP1) {
		std::cout << "WINNER: " << p2.username << std::endl;
		updateGameStats(p2, p1);
	}
	else {
		std::cout << "THE GAME IS A DRAW!" << std::endl;
		updateDrawStats(p1, p2);
	}
}

int main() {
	srand(time(0));
	std::vector<User> users = loadUsers();

	// В тази задача трябва да влязат двама играчи, за да започне играта.
	// Ще пазим индексите на влезлите играчи.
	int player1Index = -1;
	int player2Index = -1;

	while (true) {
		std::cout << "\n=== MAIN MENU ===" << std::endl;
		std::cout << "1. Register new user" << std::endl;
		std::cout << "2. Login Player 1" << (player1Index != -1 ? " (Logged: " + users[player1Index].username + ")" : "") << std::endl;
		std::cout << "3. Login Player 2" << (player2Index != -1 ? " (Logged: " + users[player2Index].username + ")" : "") << std::endl;
		std::cout << "4. Show Stats (for logged players)" << std::endl;
		std::cout << "5. START GAME (Requires both players)" << std::endl;
		std::cout << "6. Exit" << std::endl;
		std::cout << "Choose option: ";

		int choice;
		std::cin >> choice;

		if (std::cin.fail()) { // Валидация ако въведат буква вместо число
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			std::cout << "Invalid input. Please enter a number." << std::endl;
			continue;
		}

		if (choice == 1) {
			registerUser(users);
		}
		else if (choice == 2) {
			int idx = loginUser(users);
			if (idx != -1) {
				if (idx == player2Index) {
					std::cout << "Error: This user is already logged in as Player 2!" << std::endl;
				}
				else {
					player1Index = idx;
				}
			}
		}
		else if (choice == 3) {
			int idx = loginUser(users);
			if (idx != -1) {
				if (idx == player1Index) {
					std::cout << "Error: This user is already logged in as Player 1!" << std::endl;
				}
				else {
					player2Index = idx;
				}
			}
		}
		else if (choice == 4) {
			if (player1Index != -1) printStats(users[player1Index]);
			if (player2Index != -1) printStats(users[player2Index]);
			if (player1Index == -1 && player2Index == -1) std::cout << "No players logged in." << std::endl;
		}
		else if (choice == 5) {
			if (player1Index != -1 && player2Index != -1) {
				playGame(users[player1Index], users[player2Index]);
				saveUsers(users); // Записваме резултатите веднага!
			}
			else {
				std::cout << "Error: Both players must be logged in to start!" << std::endl;
			}
		}
		else if (choice == 6) {
			std::cout << "Goodbye!" << std::endl;
			break;
		}
		else {
			std::cout << "Invalid option!" << std::endl;
		}
	}

	return 0;
}