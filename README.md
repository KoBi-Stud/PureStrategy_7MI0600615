# Pure Strategy - Card Game Implementation
**Compiler used:** GCC (MinGW/CLion default) - Code is standard C++ compatible.
**Course Project: Introduction to Programming**
**Student:** Konstantin Bilalov (FN: 7MI0600615)
**Variant:** 1

## Description
This is a C++ console implementation of the card game "Pure Strategy". It supports two players competing for prize cards using their own decks. The game features user authentication, persistent data storage, and game statistics.

## Features
* **User System:** Registration and Login functionality.
* **Data Persistence:** All user data (wins, games played, stats vs specific opponents) is saved in `users_data.txt`.
* **Game Logic:**
    * Standard 52-card deck logic.
    * Fisher-Yates shuffle algorithm for the prize deck.
    * Handling of tie-breakers (prize accumulation).
* **Statistics:** Detailed breakdown of win rates against different opponents.

## How to Compile and Run
1.  Open the project in a C++ IDE (CLion, Visual Studio) or use GCC.
2.  Ensure the compiler supports C++11 or later.
3.  **Important:** Compile `main.cpp`. Ensure the working directory is set correctly so the program can create/read the database file.
4.  Run the executable.

## How to Play
1.  Start the application.
2.  Register two separate accounts (or login if they exist).
3.  Login both Player 1 and Player 2 via the menu.
4.  Select "START GAME".
5.  Follow the on-screen instructions to select cards (values 1-13).
    * Ace = 1
    * 2-10 = Face value
    * Jack = 11, Queen = 12, King = 13
