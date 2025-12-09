# Course-Project-Part-3
Casino Blackjack – C++ Project
Author: Member1 (Team Name: TN)
Date Published: (Insert your submission date here)
📌 Project Overview

This project is a single-player Casino Blackjack game written in C++, designed to simulate realistic Blackjack gameplay using correct casino rules. The program allows the player to interact with the dealer, place bets, and make gameplay decisions such as hit, stand, double down, and split pairs. The game also includes a simplified basic strategy suggestion system that helps guide the player’s decisions based on the dealer’s up card and the type of hand.

This project was developed for the CIS7 course to demonstrate understanding of algorithmic thinking, discrete structures, decision-making logic, and C++ programming techniques.

🎮 Features

✔ Play Blackjack against the dealer
✔ Full casino rules supported:

Hit

Stand

Double Down

Split Pairs

Split Aces special rules
✔ Automatic Ace handling for soft/hard hands
✔ Dealer hits until 17 and stands on 17+
✔ Blackjack pays 3:2
✔ Bankroll system with betting
✔ Multiple hands when splitting
✔ Basic strategy recommendations
✔ Clear, text-based console interface

🧠 Programming Approaches & Concepts Used
1. Object-Oriented Programming

Card, Deck, and Hand classes encapsulate behavior.

Modular structure allows clean handling of game logic.

2. Discrete Structures

Vectors to manage the deck, player hands, and split hands.

Decision trees for handling hit/stand/double/split logic.

Finite state machine flow for game phases.

3. Algorithms

Hand value calculation with dynamic Ace adjustment.

Dealer AI following standard Blackjack strategy.

Basic strategy suggestion engine (conditional logic).

4. Randomization

Uses std::shuffle and mt19937 for realistic shuffling.

📁 Repository Structure
📦 YourRepoName
 ┣ 📂 src
 │   ┗ blackjack.cpp          # Main source code
 ┣ README.md                  # Project documentation
 ┗ (Optional) Blackjack_Flowchart.drawio

▶️ How to Compile and Run
Compile (Linux/Mac/WSL):
g++ -std=c++17 blackjack.cpp -o blackjack

Run:
./blackjack

Compile on Windows (MinGW):
g++ -std=c++17 blackjack.cpp -o blackjack.exe

📝 How to Play

Start the program and read the rules summary.

You begin with a $100 bankroll.

Enter your bet at the start of each round.

The cards are dealt—one dealer card is face up.

Choose an action:

H = Hit

S = Stand

D = Double Down

P = Split Pair (if eligible)

Finish all player hands (including split hands).

Dealer reveals cards and completes their turn.

Payouts are calculated, and your bankroll is updated.

Choose whether to play another round.

💡 Example Gameplay
Your bankroll: $100
Enter your bet: 10

Dealer Shows: 9♠
Your Hand: A♥ 8♦ (19)
[Strategy Suggestion: STAND]

Choose action (H/S/D): S
Dealer draws... Dealer stands on 17
You win!
