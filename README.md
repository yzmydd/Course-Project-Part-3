# Course-Project-Part-3
Casino Blackjack – C++ Project
Author: Member1 (Team Name: TN)
Date Published: (Insert your submission date here)
Project Overview

This project is a single-player Casino Blackjack game written in C++. The program simulates realistic Blackjack gameplay using correct casino rules. The player can place bets and make decisions such as hit, stand, double down, and split pairs. The game also includes a simplified basic strategy suggestion system to guide the player based on the dealer’s up card and the player's hand.

This project was developed for the CIS7 course to demonstrate algorithmic thinking, discrete structures, decision-making logic, and C++ programming techniques.

Features

Single-player Blackjack against the dealer

Supports full casino rules:

Hit

Stand

Double Down

Split Pairs

Special rules for split Aces

Automatic Ace handling for soft and hard hands

Dealer hits until 17 and stands on 17 or more

Blackjack pays 3:2

Betting system with bankroll tracking

Handles multiple hands after splitting

Basic strategy recommendations

Console-based interface

Programming Approaches & Concepts Used
1. Object-Oriented Programming

Classes for Card, Deck, and Hand encapsulate the logic.

Modular structure supports clean and maintainable code.

2. Discrete Structures

Vectors store the deck, player hands, and split hands.

Decision trees model player choices such as hit/stand/double/split.

The game phases act like a finite state machine.

3. Algorithms

Hand value calculations including Ace adjustments.

Dealer AI implementation based on standard Blackjack rules.

Basic strategy logic using conditional decision-making.

4. Randomization

Uses std::shuffle and mt19937 to simulate a shuffled deck
