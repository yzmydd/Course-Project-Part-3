# Casino Blackjack – C++ Project

**Author:** Member1 (Team Name: TN)  
**Date Published:** *(Insert date here)*  

---

## Project Overview
This project is a single-player Casino Blackjack game written in C++. It simulates realistic Blackjack gameplay, including betting, hitting, standing, doubling down, and splitting pairs. The game also includes a simplified basic strategy suggestion system to help the player make decisions based on the dealer’s up card and the player's hand.

This project was created for the CIS7 course to demonstrate understanding of discrete structures, algorithmic design, and C++ programming concepts.

---

## Features
- Single-player Blackjack against the dealer  
- Full casino rules including hit, stand, double down, and split pairs  
- Correct handling of soft and hard hands  
- Dealer hits on 16 or less and stands on 17 or more  
- Blackjack pays 3:2  
- Bankroll system for betting  
- Supports multiple hands after splitting  
- Basic strategy recommendation system  
- Console-based interface  

---

## Programming Approaches and Concepts Used

### Object-Oriented Programming
- Classes created for Card, Deck, and Hand  
- Encapsulated logic for easier maintenance and readability  

### Discrete Structures
- Vectors used for the deck, player hands, and split hands  
- Branching game decisions represented using if/else structures  
- Game flow follows a finite state machine model  

### Algorithms
- Hand value calculation with Ace-as-1-or-11 adjustment  
- Dealer AI based on standard Blackjack rules  
- Basic strategy decision algorithm  

### Randomization
- Uses `std::shuffle` and `mt19937` to simulate realistic card shuffling  

---


## How to Play

1. Start the program from your terminal or command prompt.
2. You begin the game with a bankroll of $100.
3. At the start of each round, enter the amount you want to bet.
4. The player receives two cards, and the dealer receives two cards (one card face up).
5. Based on your hand, choose one of the following actions:
   - `H` = Hit (draw another card)
   - `S` = Stand (end your turn for that hand)
   - `D` = Double Down (double your bet and receive exactly one more card)
   - `P` = Split (only available when the first two cards have the same value)
6. If you split, each hand is played separately using the same rules.
7. After all player hands are completed, the dealer reveals their full hand.
8. The dealer hits until reaching at least 17 and stands on 17 or more.
9. The game compares each of your hands against the dealer:
   - Higher total wins  
   - Same total results in a push  
   - Going over 21 results in a bust  
10. Your bankroll is updated based on the outcome of the round.
11. Choose whether to continue playing or end the game.
